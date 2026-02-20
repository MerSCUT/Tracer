#pragma once

#include"stat_render/core/common.h"
#include"stat_render/scenes/Scene.h"
#include"stat_render/core/Camera.h"
#include"stat_render/core/Film.h"
class Renderer
{
private:
    
    Camera camera;

    float l,r,t,b;
public:
    //Renderer() {}
    Renderer(Scene scene_, Film film_, Camera camera_):
    camera(camera_) {
        float dis = 1.0f;
        t = dis * std::tan(deg2rad(camera.getFov() * 0.5f));
        r = t * camera.getAspect(); 
        l = -r;
        b = -t;
    }
    
    ~Renderer() = default;

    // Pipeline
    void RenderPipeline(const Scene& scene, Film& film);

    

    Color3f CastRay(const Ray& ray, const Scene& scene);
};