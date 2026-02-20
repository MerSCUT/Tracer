#include"stat_render/shapes/Triangle.h"

Hit Triangle::intersect(const Ray& ray)
{
    Hit payload;
    payload.intersected = false; // 确保默认状态为未击中

    Vector3f e1 = v1 - v0;
    Vector3f e2 = v2 - v0;

    // 计算射线方向与 e2 的叉积
    Vector3f pvec = Cross(ray.direct, e2);
    
    // 计算行列式 det
    float det = Dot(e1, pvec);

    // det 接近 0，说明射线与三角形所在平面平行
    // 使用 std::fabs 允许背面相交 (不剔除背面)
    if (std::fabs(det) < 1e-12) {
        return payload;
    }

    float invDet = 1.0f / det;
    Vector3f tvec = ray.origin - v0;

    // 计算重心坐标 u，并进行边界检查
    
    float u = Dot(tvec, pvec) * invDet;
    if (u < -Epsilon || u > 1.0f + Epsilon) {
        return payload;
    }

    // 计算 qvec 并求出重心坐标 v
    Vector3f qvec = Cross(tvec, e1);
    float v = Dot(ray.direct, qvec) * invDet;
    if (v < -Epsilon || u + v > 1.0f + Epsilon) {
        return payload;
    }

    // 计算射线参数 t
    float t = Dot(e2, qvec) * invDet;
    
    // 剔除位于射线起点的背后或极近处的交点 (避免自相交噪点)
    if (t < 0.0) {
        return payload;
    }

    // --- 求交成功，组装 Hit ---
    payload.intersected = true;
    payload.tmin = t;
    payload.position = ray(t);
    payload.incident = -ray.direct;
    payload.material = material;
    
    // 计算几何法线
    Vector3f normal = Cross(e1, e2).normalized();
    
    // 关键细节：如果光线从三角形背面击中，需要翻转法线
    // 这对于后续正确的物理着色（如玻璃折射、微表面反射）至关重要
    if (det < 0.0f) {
        normal = -normal;
    }
    payload.normal = normal;

    return payload;
}

bool Triangle::hit(const Ray& ray)
{
    return true;
}


