#pragma once
#include"stat_render/core/common.h"

struct Ray{
    Point3f origin;
    Vector3f direct;
    Vector3f inv_direct;

    Ray() : 
    origin(Point3f(0.,0.,0.)), 
    direct(Vector3f(0.,0.,0.)),
    inv_direct(0.,0.,0.) {}
    
    Ray(Point3f ori, Vector3f dir) : 
    origin(ori), 
    direct(dir) {
        // 防御性编程：检查方向向量是否为零或包含很小的分量
        float inv_x = std::fabs(dir.x()) > 1e-9f ? 1.0f / dir.x() : Inf;
        float inv_y = std::fabs(dir.y()) > 1e-9f ? 1.0f / dir.y() : Inf;
        float inv_z = std::fabs(dir.z()) > 1e-9f ? 1.0f / dir.z() : Inf;
        inv_direct = Vector3f(inv_x, inv_y, inv_z);
    }

    Point3f operator()(float t) const { return  t * direct + origin ; }
};
