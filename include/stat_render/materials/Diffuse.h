#pragma once
#include"stat_render/core/common.h"
#include"stat_render/materials/Material.h"

class Diffuse : public Material{
private:
    Color3f Kd;     // 漫反射系数
public:
    Vector3f sample(const Vector3f& wi, const Vector3f& n) override;
    
    float pdf(const Vector3f & wi, const Vector3f & wo, const Vector3f& n);
    
    Color3f eval(const Vector3f & wi, const Vector3f& wo, const Vector3f & n);
};