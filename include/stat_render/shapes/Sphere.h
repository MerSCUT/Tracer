#pragma once

#include"stat_render/shapes/Object.h"

class Sphere :public Object{
private:
    Point3f center;
    float radius;
    Bound bound;
    Material* material;
public:
    Sphere() : radius(0.0f), center(Point3f(0.f,0.f,0.f)) {}
    Sphere(Point3f cen, float r) : radius(r), center(cen) {}
    ~Sphere() = default;


    Bound getBound() override {return bound;}
    Material* getMaterial() override {return material;}
    float getSurfaceArea();
    bool hit(const Ray & ray);     
    Hit intersect(const Ray& ray) override;
    inline float SurfaceArea() override { return 4.f * Pi * radius * radius; }
    inline Point3f getCenter() override { return center; }
};