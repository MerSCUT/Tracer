#pragma once

#include"stat_render/core/common.h"
#include"stat_render/scenes/Scene.h"
#include"stat_render/core/Camera.h"
#include"stat_render/core/Film.h"

enum class Mode { PathTracing, NormalMap };
class Renderer
{
private:
    
    Camera camera;

    float l,r,t,b;
    Mode mode;

public:
    //Renderer() {}
    Renderer(Scene scene_, Film film_, Camera camera_, Mode mode_ = Mode::PathTracing):
    camera(camera_), mode(mode_) {
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