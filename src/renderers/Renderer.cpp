#include"stat_render/renderers/Renderer.h"
#include<iostream>
void Renderer::RenderPipeline(const Scene& scene, Film& film)
{
    int progress = 0;
    for(int j = 0; j < film.getHeight(); j++)
    {
        for(int i = 0; i < film.getWidth(); i++)
        {
            float breakpoint;
            if (i == 129 && j == 175)
            {
                breakpoint = 1;
            }
            float u = l + (float)(i+0.5f)/(float)(film.getWidth()) * (r-l);
            float v = -b - (float)(j+0.5f)/(float)(film.getHeight()) * (t-b);
            // Ray generation
            Ray ray(Point3f(camera.getPosition()), Vector3f(u, v, -1));
            auto res = CastRay(ray, scene);
            film.set(i,j, res);
            progress++;
            std::cout << "\r进度: " << progress << " / " << film.getHeight() * film.getWidth() << std::flush;
        }
    }
    std::cout << std::endl;
}

Color3f Renderer::CastRay(const Ray& ray, const Scene& scene)
{
    if (Hit temp = scene.intersect(ray); temp.intersected)
    {
        // normal
        switch (mode)
        {
        case Mode::NormalMap :
            
            auto n = temp.normal;
            // [-1,1] -> [0,1]
            
            float R = n.x() * 0.5f + 0.5f;
            float G = n.y() * 0.5f + 0.5f;
            float B = n.z() * 0.5f + 0.5f;
            
            return Color3f(255. * R, 255. * G,255. * B);

            break;
        }


        return Color3f(255.0f, 0.f, 0.f);
    }
    return Color3f(199.f,233.f,233.f);
}