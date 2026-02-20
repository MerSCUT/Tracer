#pragma once
#include"stat_render/core/common.h"
#include"stat_render/core/Camera.h"
// 

Eigen::Matrix4f ViewTransform(const Camera& camera)
{
    auto g = camera.getGaze();
    auto t = camera.getTop();
    auto p = camera.getPosition();
    
    Eigen::Matrix4f translation;
    translation << 1., 0., 0., -p.x(),
                0., 1., 0., -p.y(),
                0., 0., 1., -p.z(),
                0., 0., 0., 1.;
    Eigen::Matrix4f rotation;
    auto gCrosst = Cross(g, t);
    rotation << gCrosst.x(), t.x(), g.x(), 0.,
                gCrosst.y(), t.y(), g.y(), 0.,
                gCrosst.z(), t.z(), g.z(), 0.,
                0., 0., 0., 1.;
    Eigen::Matrix4f View;
    View = rotation * translation;
    return View;
}

Eigen::Matrix4f PerspectiveTransform()
{
    // 返回
}
