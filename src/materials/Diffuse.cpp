#include"stat_render/materials/Diffuse.h"
#include<random>
Vector3f Diffuse::sample(const Vector3f& wi, const Vector3f& n)
{
    // 均匀采样单位半球
    // 局部坐标系中先采样, 再变换到实际采样点.
    
    Sampler sampler;
    float u1 = sampler.get1D();
    float u2 = sampler.get1D();
    float theta = std::acos(u1);
    float phi = 2.0f * Pi * u2;


    float sin_theta = std::sin(theta);
    float x = sin_theta * std::cos(phi);
    float y = sin_theta * std::sin(phi);
    float z = std::cos(theta); // 即 u1

    Vector3f t;
    if (std::abs(n.x()) > std::abs(n.y())) {
        float invLen = 1.0f / std::sqrt(n.x() * n.x() + n.z() * n.z());
        t = Vector3f(n.z() * invLen, 0.0f, -n.x() * invLen);
    } else {
        float invLen = 1.0f / std::sqrt(n.y() * n.y() + n.z() * n.z());
        t = Vector3f(0.0f, n.z() * invLen, -n.y() * invLen);
    }
    // 通过叉乘得到副切向量 b
    Vector3f b = n.cross(t);

    return x * t + y * b + z * n;
}

float Diffuse::pdf(const Vector3f & wi, const Vector3f & wo, const Vector3f& n)
{
    // 给定 wo, n, 采样到 wi 的概率.
    return 0.5f * inv_Pi;
}
    
Color3f Diffuse::eval(const Vector3f & wi, const Vector3f& wo, const Vector3f & n)
{
    // BRDF 常数.
    // 确保 wo, wi 方向朝外
    if (Dot(n, wi) > 0.0f && Dot(n, wo) > 0.0f)
    {
        return albedo * inv_Pi;
    }
    return Color3f(0.0f, 0.0f, 0.0f);
}