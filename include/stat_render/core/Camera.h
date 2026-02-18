#pragma once
#include"stat_render/core/common.h"
#include"Film.h"
class Camera 
{
private:
    Point3f pos;
    Vector3f right, up;
    float aspect;
    float fov;      // deg
public:

    Camera(Point3f p, Vector3f r, Vector3f u, float asp = 1., float _fov = 90.) :
    pos(p), right(r), up(u), aspect(asp), fov(_fov) {}

    Camera() : 
    pos(Point3f(0.,0.,0.)),
    right(Vector3f(1., 0., 0.)),
    up(Vector3f(0., 1., 0.)),
    aspect(1.),
    fov(90.) {}

    

    Camera(const Film& film) : Camera(Point3f(0.,0.,0.), Vector3f(1., 0., 0.), Vector3f(0., 1., 0.), film.getWidth()/film.getHeight(), 90)  {}

    Camera(Point3f p, Vector3f r, Vector3f u, const Film& film, float fov_ = 90) :
    Camera(p, r, u, film.getWidth()/film.getHeight(), fov_) {}


    ~Camera() = default;

    Point3f getPosition() const { return pos; }
    float getAspect() const { return aspect; }
    float getFov() const { return fov; }
};