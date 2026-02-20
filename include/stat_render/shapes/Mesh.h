#pragma once

#include"stat_render/shapes/Triangle.h"
#include"stat_render/accelerators/BVH.h"
#include<vector>
#include<memory>
class Mesh : public Object{
private:
    std::vector<Object*> triangles;
    Material* m;
    Bound bound;
    BVH* bvh;
public:
    Mesh(std::vector<Object*> triangles_) : triangles(triangles_) {}

    Mesh(const std::vector<Vector3f>& vertices, 
         const std::vector<uint32_t>& indices, 
         Material* m);

    Hit intersect(const Ray& ray) override;
    Hit intersectWithoutBVH(const Ray& ray) ;
    float SurfaceArea() override { return 1.0f; };

    Bound getBound() override {return bound; }
    Material* getMaterial() override {return m; }
    
    inline Point3f getCenter() override { return getBound().Center(); }
    ~Mesh();
};