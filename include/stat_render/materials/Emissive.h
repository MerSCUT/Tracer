#pragma once
#include"stat_render/core/common.h"
#include"stat_render/materials/Material.h"
#include"stat_render/textures/Texture.h"
class Emissive : public Material
{
private:
    Vector3f emission;          // 基础发光强度
    Texture* map;               // 纹理
public:
    Emissive() : 
    Material(MaterialType::Emissive), 
    map(nullptr),
    emission(Vector3f(0.,0.,0.)) {}
    
    Emissive(const Vector3f& emission_) : 
    Material(MaterialType::Emissive),
    map(nullptr), 
    emission(emission_) {}

    Vector3f getEmission() const { return emission; }
};