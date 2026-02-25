#pragma once
#include<vector>
#include"stat_render/shapes/Object.h"
#include"stat_render/lights/AreaLights.h"
#include"stat_render/materials/Diffuse.h"
#include"stat_render/materials/Emissive.h"
#include"stat_render/scenes/parser.h"
#include<iostream>
#include<memory>
class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    std::vector<std::shared_ptr<Material>> material_pool;
    Mat4f M_normalize;
public:
    Scene() {}
    
    ~Scene() = default;
    void AddObject(Object* object) { objects.push_back(object); }
    void AddLight(Light* light) { lights.push_back(light); }
    Mat4f getNormalizeMatrix() { return M_normalize; }

    void loadOBJlist(const std::vector<std::string>& paths, const std::vector<Color3f>& emissions, const std::vector<DiffuseColor>& dcs)
    {
        Bound bound;
        assert(paths.size() == emissions.size() && paths.size() == dcs.size());
        for(int n = 0; n < paths.size(); n++)
        {
            this->loadOBJ(paths[n], bound, emissions[n], dcs[n]);
        }
        M_normalize = bound.getNormalizeMatrix();
        bound = Bound();
        for(auto& obj : objects)
        {
            obj->transform(M_normalize);
            
            bound.Union(obj->getBound());
        }
        std::cout << bound.getPmin() << '\n' << bound.getPmax() << std::endl;
        return;
    }

    void loadOBJ(const std::string& path,  Bound& boundbox, const Color3f& emission = Color3f(0.0f, 0.0f, 0.0f), const DiffuseColor dc = DiffuseColor::WHITE)
    {
        std::shared_ptr<Material> m;
        if (emission.isZero())
        {
            m = std::make_shared<Diffuse>(dc);
        }
        else
        {
            m = std::make_shared<Emissive>(emission);
        }
        auto mesh = Parser::loadOBJ(path, m);
        AddObject(mesh);
        boundbox.Union(mesh->getBound());
        if (!emission.cwiseEqual(0.0f).all()) 
        {
            Light* l = new AreaLight(mesh, emission);
            AddLight(l);
        }
        material_pool.push_back(m);
        return;
    }
    
    inline std::vector<Object*> getObjects() const { return objects; }
    inline std::vector<Light*> getLights() const { return lights; }
    
    Hit intersect(const Ray& ray) const;
    // 在 light 列表里采样
    LightSample sampleLight() const
    {
        LightSample ls;
        // 假设场景中只有一个光源
        assert(lights.size() == 1);
        auto l = lights[0];
        Sampler sampler;
        Vector2f u = sampler.get2D();
        ls = l->sampleLight(u.x(), u.y());
        return ls;
        
        // ... 多个光源 :
    }
};