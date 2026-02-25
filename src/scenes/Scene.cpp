#include"stat_render/scenes/Scene.h"
#include<iostream>
Hit Scene::intersect(const Ray& ray) const
{
    Hit payload;
    for(auto& obj : objects)
    {
        if (Hit temp = obj->intersect(ray); 
            temp.intersected && temp.tmin <= payload.tmin)
        {
            payload = temp;
        }
    }
    return payload;
}
void Scene::loadOBJlist(const std::vector<std::string>& paths, const std::vector<Color3f>& emissions, const std::vector<DiffuseColor>& dcs)
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
    // std::cout << bound.getPmin().x << ' ' << bound.getPmin().y << ' ' << bound.getPmin().z << std::endl;
    return;
}


void Scene::loadOBJ(const std::string& path,  Bound& boundbox, const Color3f& emission, const DiffuseColor dc)
{
    std::shared_ptr<Material> m;
    if (emission == Color3f(0.f))
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
    if (!(emission == Color3f(0.f))) 
    {
        Light* l = new AreaLight(mesh, emission);
        AddLight(l);
    }
    material_pool.push_back(m);
    return;
}

LightSample Scene::sampleLight() const
{
    LightSample ls;
    // 假设场景中只有一个光源
    assert(lights.size() == 1);
    auto l = lights[0];
    Sampler sampler;
    Vec2f u = sampler.get2D();
    ls = l->sampleLight(u.x, u.y);
    return ls;
    
    // ... 多个光源 :
}