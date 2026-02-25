#include"stat_render/renderers/Renderer.h"
#include"stat_render/core/transform.h"
#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
const int tile_size = 32;        // Tile size
void Renderer::RenderMultiThreading(const Scene& scene, Film& film)
{
    int spp = SPP;      // from common.h
    int progress = 0;
    int w = film.getWidth();
    int h = film.getHeight();
    int total = w * h;
    

    struct Tile{ 
        int row_st, col_st, row_ed, col_ed;         // [st, ed)
    };
    // 渲染分块
    std::vector<Tile> tiles;
    for(int j = 0; j < h; j += tile_size)
    {
        for(int i = 0; i < w; i += tile_size)
        {
            tiles.push_back(
                { i, j, std::min(i + tile_size, w), std::min(j + tile_size, h) }
            );
        }
    }

    std::atomic<int> globalProgress{0};
    int nextTile = 0;
    std::mutex tileMutex;

    auto worker = [&]()
    {
        while(1)
        {
            // 从 tiles 中取出下一个
            Tile tile;
            {
                std::unique_lock<std::mutex> lock{tileMutex};
                if (nextTile >= tiles.size()) break;
                tile = tiles[nextTile++];
            }
            int localProgress = 0;

            for(int j = tile.col_st; j < tile.col_ed; j++)
            {
                for(int i = tile.row_st; i < tile.row_ed; i++)
                {
                    float u = l + (float)(i+0.5f)/(float)(film.getWidth()) * (r-l);
                    float v = -b - (float)(j+0.5f)/(float)(film.getHeight()) * (t-b);
                    // Ray generation in Camera Standard Space
                    Vector3f dir = Vector3f(u, v, -1);
                    // Transform to world space;
                    auto M = CameraToWorldTransform(camera.getPosition(), camera.getGaze(), camera.getTop());
                    Vector4f ret = M * VectorTo4D(dir);
                    dir = Vector3f(ret.x(), ret.y(), ret.z()).normalized();
                    Ray ray(Point3f(camera.getPosition()), dir);
                    // Start rendering
                    Color3f result = Color3f(0.0f,0.0f,0.0f);
                    for(int s = 0; s < spp; s++)
                    {
                        auto res = CastRay(ray, scene, 0);
                        result += res/(float)spp;
                    }
                    film.add(i,j, result);
                    localProgress++;
                }
                
            }
            globalProgress += localProgress;
            localProgress = 0;
        }
        
    };

    int numThreads = std::thread::hardware_concurrency() - 10;
    std::cout << "[Info] Find " << numThreads << " kernels" << std::endl;
    std::vector<std::thread> threads;
    for(int k = 0; k < numThreads; k++)
    {
        threads.emplace_back(worker);
    }

    while (globalProgress < total) {
        int current = globalProgress.load();
        float percent = (float)current / total * 100.0f;
        std::cout << "\r进度: " << current << " / " << total 
                  << " (" << percent << "%)    " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 0.1s interval
    }
    for(int i = 0; i < numThreads; i++)
    {
        threads[i].join();
    }
    
    return;
}

void Renderer::RenderPipeline(const Scene& scene, Film& film)
{
    int spp = SPP;          // from common.h
    int progress = 0;
    for(int j = 0; j < film.getHeight(); j++)
    {
        for(int i = 0; i < film.getWidth(); i++)
        {
            
            float u = l + (float)(i+0.5f)/(float)(film.getWidth()) * (r-l);
            float v = -b - (float)(j+0.5f)/(float)(film.getHeight()) * (t-b);
            // Ray generation
            Vector3f dir = Vector3f(u, v, -1);
            auto M = CameraToWorldTransform(camera.getPosition(), camera.getGaze(), camera.getTop());
            Vector4f ret = M * VectorTo4D(dir);
            dir = Vector3f(ret.x(), ret.y(), ret.z()).normalized();
            Ray ray(Point3f(camera.getPosition()), dir);

            for(int s = 0; s < spp; s++)
            {
                auto res = CastRay(ray, scene, 0);
                
                film.add(i,j, res/(float)spp);
            }
            
            
            progress++;
            std::cout << "\r进度: " << progress << " / " << film.getHeight() * film.getWidth() << std::flush;
        }
    }
    std::cout << std::endl;
}

Color3f Renderer::CastRay(const Ray& ray, const Scene& scene, int depth)
{
    if (depth > 5) return Color3f(0.f, 0.f, 0.f);
    if (Hit payload = scene.intersect(ray);  
    payload.intersected)
    {
        switch (mode)
        {
        case Mode::PathTracing :
        {
            // 光源
            Object* ob = payload.obj;
            ob->getBound();
            Material* w = payload.obj->getMaterial();
            if (payload.obj->getMaterial()->isEmissive())
            {
                if (depth == 0)
                return payload.obj->getMaterial()->getEmission();
                else 
                return Color3f(0.f,0.f,0.f);
            }
            
            // 非光源
            // 1. 直接光照
            Color3f L_dir(0.0f, 0.0f, 0.0f);
            LightSample ls = scene.sampleLight();
            auto p = payload.position;
            auto l = ls.position;
            Vector3f wi = (l - p).normalized();    // outwards
            auto n_p = payload.normal;
            auto n_l = ls.normal;
            // 相交测试
            Ray test(p + Epsilon * n_p, wi);

            auto hit_test = scene.intersect(test);
            // if ((test(hit_test.tmin) - payload.position).norm() > (ls.position - payload.position).norm())
            
            if ((test(hit_test.tmin) - p).norm() + Epsilon >= (l-p).norm()) 
            {
                
                Vector3f wo = payload.incident.normalized();
                

                // 根据 payload.material 计算 L_o
                // L_o = fr * L_i * Dot(n, wi) * Dot(-n_l, wi) / Dot(p-l, p-l) / ls.pdf_A
                auto fr = payload.material->eval(wi, wo, n_p);
                auto Li = ls.radiance;
                auto cos_thetai = Dot(n_p, wi);
                cos_thetai = cos_thetai > 0.0f ? cos_thetai : 0.0f;
                auto cos_thetaip = std::abs(Dot(n_l, wi));
                

                //assert(cos_thetaip >= 0.f);
                auto dis = Dot(p-l, p-l);
                //assert(cos_thetai >= 0.0f);
                //assert(cos_thetaip >= 0.0f);
                L_dir = fr * Li * cos_thetai * cos_thetaip / (dis * ls.pdf) ;
            }

            Color3f L_indir(0.f, 0.f, 0.f);
            // Russian Roulette
            float p_rr = 0.8f;
            Sampler sampler;
            auto u = sampler.get1D(); 
            if (u >= p_rr) return L_dir;
            Vector3f wo = (ray.origin - p).normalized();
            // 采样入射方向 wi (局部)
            Vector3f wi_ind = payload.material->sample(wo, n_p).normalized();
            float pdf_ind = payload.material->pdf(wi_ind, wo, n_p);
            pdf_ind = std::max(pdf_ind, 1e-3f);

            Ray ray_ind(p + Epsilon * n_p, wi_ind);
            {
                auto fr = payload.material->eval(wi_ind, wo, n_p);
                auto costhetai = Dot(n_p, wi_ind);
                auto Li = CastRay(ray_ind, scene, depth+1);
                L_indir = fr * Li * costhetai / (pdf_ind * p_rr);
                
                if (L_indir.x() >= 1.f || L_indir.y() >= 1.f
                || L_indir.z() >= 1.f )
                {
                    int breakpoint1 = 10;
                    int breakpoint = 10;
                }
            }
            
            return L_dir + L_indir;
            
            
            break;
        }
            
        // normal
        case (Mode::NormalMap) :
        {
            auto n = payload.normal;
            // [-1,1] -> [0,1]
            
            float R = n.x() * 0.5f + 0.5f;
            float G = n.y() * 0.5f + 0.5f;
            float B = n.z() * 0.5f + 0.5f;
            
            return Color3f(255. * R, 255. * G,255. * B);

            break;
        }
            
            
        }
        return Color3f(1.f, 0.f, 0.f);
    }
    return Color3f(0.f, 0.f, 0.f);
    return Color3f(199.f/255.f,233.f/255.f,233.f/255.f);
}