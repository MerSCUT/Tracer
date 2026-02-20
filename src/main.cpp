#include<iostream>
#include<random>
#include"stat_render/core/Film.h"
#include"stat_render/core/Ray.h"
#include"stat_render/shapes/Sphere.h"
#include"stat_render/core/Camera.h"
#include"stat_render/scenes/Scene.h"
#include"stat_render/shapes/Triangle.h"
#include"stat_render/renderers/Renderer.h"
#include<ctime>
void test_outputImage()
{
    Film film(32,32);
    //film.OutputImage();
    return;
}
void test_triangle()
{
    Triangle tri;//(Point3f(-0.5f, -0.5f, -4.0), Point3f(0.5f, -0.5f, -8.0f), Point3f(0.0f, 0.5f , -4.0f));
    Film film;
    // default set
    Camera camera(film);
    Scene scene;
    Object* obj = &tri;
    scene.AddObject(obj);
    
    
    auto p = camera.getPosition();
    auto fov = camera.getFov();
    auto asp = camera.getAspect();
    float l,r,t,b;
    float dis = 1.0f;

    // Ray generation
    t = dis * std::tan(deg2rad(fov * 0.5f));
    r = t * asp; 
    l = -r;
    b = -t;
    // Integrator
    for(int j = 0; j < film.getHeight(); j++)
    {
        for(int i = 0; i < film.getWidth(); i++)
        {
            // center of pixels
            float u = l + (float)(i+0.5)/(float)(film.getWidth()) * (r-l);
            float v = b + (float)(j+0.5)/(float)(film.getHeight()) * (t-b);
            v = -v;
            // Ray generation
            Ray ray(Point3f(p), Vector3f(u, v, -1));
            
            if (Hit payload = tri.intersect(ray); payload.intersected){
                
                film.set(i, j, Color3f(255., 0.,0.));
            }
            else{
                film.set(i,j, Color3f(0., 0., 255.));
            }
        }
    }
    film.Write("../images/test_triangle.ppm");
}
void test_sphere()
{
    Sphere sph( Point3f(0.f, 0.f, -2.f), 0.5f);
    Film film;
    // default set
    Camera camera(film);
    Scene scene;
    Object* obj = &sph;
    scene.AddObject(obj);
    
    
    auto p = camera.getPosition();
    auto fov = camera.getFov();
    auto asp = camera.getAspect();
    float l,r,t,b;
    float dis = 1.0f;

    // Ray generation
    t = dis * std::tan(deg2rad(fov * 0.5f));
    r = t * asp; 
    l = -r;
    b = -t;
    // Integrator
    for(int j = 0; j < film.getHeight(); j++)
    {
        for(int i = 0; i < film.getWidth(); i++)
        {
            // center of pixels
            float u = l + (float)(i+0.5)/(float)(film.getWidth()) * (r-l);
            float v = b + (float)(j+0.5)/(float)(film.getHeight()) * (t-b);
            v = -v;
            // Ray generation
            Ray ray(Point3f(p), Vector3f(u, v, -1));

            if (Hit payload = sph.intersect(ray); payload.intersected){
                film.set(i, j, Color3f(255., 0.,0.));
            }
            else{
                film.set(i,j, Color3f(0., 0., 255.));
            }
        }
    }
    film.Write("../images/test_sphere.ppm");
}

void test_loadOBJ()
{
    Scene scene;
    std::string path("../asset/bunny/bunny.obj");
    scene.loadOBJ(path);
    Bound b = scene.getObjects()[0]->getBound();
    std::cout << b.Center() << std::endl;
    std::cout << "Pmin : " << b.getPmin() << std::endl;
    std::cout << "Pmax : " << b.getPmax() << std::endl;
    Film film(64,64);
    Point3f position(-0.016826f, 0.110153f, 0.4f);
    // 视线右侧方向向量 (X轴正向)
    Vector3f right(1.0f, 0.0f, 0.0f);

    // 视线顶上的方向 (Y轴正向)
    Vector3f up(0.0f, 1.0f, 0.0f);
    Camera camera(position, right, up, film);
    Renderer renderer(scene, film, camera);
    renderer.RenderPipeline(scene, film);

    film.Write("../images/test_OBJloadeAndRenderer.ppm");

    std::cout << "Finish" << std::endl;
    return;
}

void test_BVH()
{
    Scene scene;
    std::string path("../asset/bunny/bunny.obj");
    scene.loadOBJ(path);
    Bound b = scene.getObjects()[0]->getBound();
    
    Film film(512,512);
    Point3f position(0.f, 0.f, 3.0f);
    // 视线方向
    Vector3f gaze(0.0f, -1.0f, -1.0f);

    // 视线顶上的方向 (Y轴正向)
    Vector3f up(0.0f, 1.0f, 0.0f);
    Camera camera(position, gaze, up, film);
    Renderer renderer(scene, film, camera, Mode::NormalMap);
    renderer.RenderPipeline(scene, film);

    film.Write("../images/test_BVH.ppm");

    std::cout << "Finish" << std::endl;
    return;
}

int main()
{
    test_BVH();
    return 0;
    
    std::random_device rd;
    unsigned int seed = rd();
    std::mt19937 gen(seed);

    std::uniform_real_distribution<> dis_real(0.0, 1.0);
    for(int i = 0; i < 10; i++)
    {
        std::cout << " " << gen() % 100;
    }
    std::cout << '\n';
    //return 0;
    std::cout << "Test_Finished" << std::endl;
    return 0;
}


