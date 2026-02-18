#include"stat_render/materials/Diffuse.h"
#include<random>
Vector3f Diffuse::sample(const Vector3f& wi, const Vector3f& n)
{
    // 均匀采样单位半球
    // 局部坐标系中先采样, 再变换到实际采样点.
    std::mt19937 gen(42);
    std::uniform_real_distribution uniform(0.0,1.0);
    float u1 = uniform(gen);
    float u2 = uniform(gen);
    float theta = std::acos(u1);
    float phi = 2 * Pi * u2;
    Vector3f local  = SphTo3D(theta,phi);
    return local;
}

float Diffuse::pdf(const Vector3f & wi, const Vector3f & wo, const Vector3f& n)
{
    // uniformly sampling
    return 0.5f * inv_Pi;
}
    
Color3f Diffuse::eval(const Vector3f & wi, const Vector3f& wo, const Vector3f & n)
{
    // uniformly reflecting
    float ans = 0.5f * inv_Pi;
    return Color3f(ans,ans,ans);
}