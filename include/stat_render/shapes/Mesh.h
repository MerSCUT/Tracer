#pragma once

#include"stat_render/shapes/Triangle.h"
#include<vector>
#include<memory>
class Mesh : public Object{
private:
    std::vector<std::shared_ptr<Object>> triangles;
    Material* m;
    Bound bound;
public:
    Mesh(std::vector<std::shared_ptr<Object>> triangles_) : triangles(triangles_) {}

    Mesh(const std::vector<Vector3f>& vertices, 
         const std::vector<uint32_t>& indices, 
         Material* m);

    Hit intersect(const Ray& ray) override;
    float SurfaceArea() override { return 1.0f; };

    Bound getBound() override {return bound; }
    Material* getMaterial() override {return m; }
    
    ~Mesh() = default;
};