#include"stat_render/renderers/Renderer.h"
#include<iostream>
void Renderer::RenderPipeline(Film& film)
{
    int progress = 0;
    for(int j = 0; j < film.getHeight(); j++)
    {
        for(int i = 0; i < film.getWidth(); i++)
        {
            // center of pixels
            float u = l + (float)(i+0.5f)/(float)(film.getWidth()) * (r-l);
            float v = -b - (float)(j+0.5f)/(float)(film.getHeight()) * (t-b);
            // Ray generation
            Ray ray(Point3f(camera.getPosition()), Vector3f(u, v, -1));
            auto res = CastRay(ray);
            film.set(i,j, res);
            progress++;
            std::cout << "\r进度: " << progress << " / " << film.getHeight() * film.getWidth() << std::flush;
        }
    }
}

Color3f Renderer::CastRay(const Ray& ray)
{
    if (Hit temp = scene.intersect(ray); temp.intersected)
    {
        return Color3f(125.0f, 0.f, 0.f);
    }
    return Color3f(0.f,0.f,125.f);
}