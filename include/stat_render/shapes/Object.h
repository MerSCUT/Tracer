#pragma once
#include"stat_render/core/Ray.h"
#include"stat_render/core/Hit.h"
#include"stat_render/materials/Material.h"
#include"stat_render/accelerators/Bound.h"
// 物体抽象基类
class Object{
public:
    // API
    
    // 1. Ojbect x Ray
    virtual Hit intersect(const Ray& ray) = 0;
    // 2. Bounding Box
    virtual Bound getBound() = 0;
    virtual Material* getMaterial() = 0;
    Object() {}
    
    virtual ~Object() = default;

    virtual float SurfaceArea() = 0;
    virtual Point3f getCenter() = 0;
};