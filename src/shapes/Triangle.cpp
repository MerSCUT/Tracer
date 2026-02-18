#include"stat_render/shapes/Triangle.h"

Hit Triangle::intersect(const Ray& ray)
{
    Hit payload;
    Vector3f normal = Cross(v1-v0, v2-v0).normalized();
    
    float t = Dot(v0 - ray.origin , normal)/Dot(ray.direct, normal);
    if (t < -Epsilon) return payload;

    Vector3f t1 = Cross(ray(t)-v0, v1-v0).normalized();
    Vector3f t2 = Cross(ray(t)-v1, v2-v1).normalized();
    Vector3f t3 = Cross(ray(t)-v2, v0-v2).normalized();
    bool con1 = Dot(t1, normal) >= -Epsilon &&  Dot(t2,normal) >= -Epsilon && Dot(t3, normal) >= -Epsilon;
    bool con2 = Dot(t1, normal) <= Epsilon &&  Dot(t2,normal) <= Epsilon && Dot(t3, normal) <= Epsilon;
    auto r1 = Dot(t1,normal);
    auto r2 = Dot(t2,normal);
    auto r3 = Dot(t3,normal);
    if (!(r1 >= -Epsilon && r2 >= -Epsilon && r3 >= -Epsilon)
        && !(r1 <= Epsilon && r2 <= Epsilon && r3 <= Epsilon)) 
    return payload;
    
    

    payload.intersected = true;
    payload.position = ray(t);
    payload.material = material;
    //payload.normal = n_interp; 法向量插值
    payload.tmin = t;
    payload.incident = -ray.direct;
    return payload;
}

bool Triangle::hit(const Ray& ray)
{
    return true;
}