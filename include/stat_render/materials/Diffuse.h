#pragma once
#include"stat_render/core/common.h"
#include"stat_render/materials/Material.h"

enum class DiffuseColor { BLUE, GREEN, RED, WHITE };


class Diffuse : public Material{
private:
    Color3f albedo = Color3f(0.f, 0.f, 0.f);     // 漫反射系数
public:
    Diffuse() = default;
    Diffuse(Color3f k) : albedo(k) {}
    Diffuse(DiffuseColor dc) 
    {
        switch (dc)
        {
        case DiffuseColor::BLUE :
        {
            albedo = Color3f(0.14, 0.14, 0.45);
            break;
        }
        case DiffuseColor::GREEN :
        {
            albedo = Color3f(0.14, 0.45, 0.091);
            break;
        }
        case DiffuseColor::WHITE :
        {
            albedo = Color3f(0.725, 0.71, 0.68);
            break;
        }
        case DiffuseColor::RED :
        {
            albedo = Color3f(0.63, 0.065, 0.05);
            break;
        }
        }
    }

    Vector3f sample(const Vector3f& wi, const Vector3f& n) override;
    
    float pdf(const Vector3f & wi, const Vector3f & wo, const Vector3f& n);
    
Color3f eval(const Vector3f & wi, const Vector3f& wo, const Vector3f & n);
};