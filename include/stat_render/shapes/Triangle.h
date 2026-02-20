#pragma once

#include"stat_render/shapes/Object.h"
class Triangle : public Object
{
private:
    Point3f v0, v1, v2;
    Vector3f n0, n1, n2;   //vertices normal
    Point2f uv0, uv1, uv2;
    Bound bound;
    Material* material;
public:
    // All zero by default
    Triangle() : 
    v0(Point3f(0.,0.,0.)),
    v1(v0),
    v2(v0),
    uv0(Point2f(0.,0.)),
    uv1(uv0),
    uv2(uv0),
    bound(Bound()), 
    material(nullptr) {
        Vector3f pmin = (v0.array() < v1.array()).select(v0, v1);
        pmin = (pmin.array() < v2.array()).select(pmin,v2);

        Vector3f pmax = (v0.array() > v1.array()).select(v0,v1);
        pmax = (pmax.array() > pmax.array()).select(pmax,v2);

        bound = Bound(pmin, pmax);
    }

    Triangle(Point3f v0_, Point3f v1_, Point3f v2_, Material* m) 
    : v0(v0_), v1(v1_), v2(v2_), material(m) {
        
        // 使用 Eigen 内置的系数级(coeff-wise)比较
        Vector3f pmin = v0.cwiseMin(v1).cwiseMin(v2);
        Vector3f pmax = v0.cwiseMax(v1).cwiseMax(v2);

        // 此时调用 Bound 的构造函数
        // 务必确保 Bound 的构造函数内部执行了“膨胀(Padding)”逻辑
        bound = Bound(pmin, pmax); 
    }

    

    Triangle(Point3f v0_, Point3f v1_, Point3f v2_,
    Vector3f n0_, Vector3f n1_, Vector3f n2_,
    Point2f uv0_, Point2f uv1_, Point2f uv2_, Material* m) :
    v0(v0_), v1(v1_), v2(v2_), 
    n0(n0_), n1(n1_), n2(n2_),
    uv0(uv0_), uv1(uv1_), uv2(uv2_),
    material(m) {
        // 用顶点进行 bound 初始化

        // Eigen::Vector3f 没有逐元素运算, 需要转为 array
        Vector3f pmin = (v0.array() < v1.array()).select(v0, v1);
        pmin = (pmin.array() < v2.array()).select(pmin,v2);

        Vector3f pmax = (v0.array() > v1.array()).select(v0,v1);
        pmax = (pmax.array() > pmax.array()).select(pmax,v2);

        bound = Bound(pmin, pmax);
    }

    ~Triangle() = default;

    Bound getBound() override {return bound;}
    Material* getMaterial() override {return material;}
    

    inline void setVertex(const Point3f& _v0, const Point3f& _v1, const Point3f& _v2)
    {
        v0 = _v0 ; v1  = _v1; v2 = _v2;
    }
    inline void setMaterial(Material* m) { this->material = m; }
    
    Hit intersect(const Ray& ray) override;
    bool hit(const Ray& ray);

    inline float SurfaceArea() override { return 1.0f; }
    inline Point3f getCenter() override {  
        return (v0 + v1 + v2) * 1./3.; 
    }

    Hit getHit(const Ray& ray);
};