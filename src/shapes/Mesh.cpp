#include"stat_render/shapes/Mesh.h"

Mesh::Mesh(const std::vector<Vector3f>& vertices, 
         const std::vector<uint32_t>& indices, 
         Material* m) 
{
    // 1. 根据索引数据创建 Triangle 实例
    Bound b;
    for (size_t i = 0; i < indices.size(); i += 3) {
        auto t = new Triangle(
            vertices[indices[i]], 
            vertices[indices[i+1]], 
            vertices[indices[i+2]], 
            m
        );
        triangles.push_back(t);
        b.Union(t->getBound());
    }
    
    this->bound = b;
    this->bvh = new BVH(triangles); 
}

// Without BVH acceleration
Hit Mesh::intersectWithoutBVH(const Ray& ray)
{
    Hit payload;
    for(auto& tri : triangles)
    {
        if (Hit temp = tri->intersect(ray); temp.intersected && temp.tmin < payload.tmin)
        payload = temp;
    }
    return payload;
}

Hit Mesh::intersect(const Ray& ray)
{
    //return intersectWithoutBVH(ray);
    return bvh->intersect(ray);
}

Mesh::~Mesh()
{
    // triangles 里的 std::vector<Object*> 要全部删除.
    for(auto& obj : triangles)
    {
        delete obj;
    }
}