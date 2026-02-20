#pragma once
#include<vector>
#include"stat_render/shapes/Object.h"
#include"stat_render/lights/Light.h"
#include"stat_render/materials/Diffuse.h"
#include"stat_render/scenes/parser.h"
#include<memory>
class Scene
{
private:
    
    std::vector<Object*> objects;
    std::vector<Light*> lights;
public:
    Scene() {}
    
    ~Scene() = default;
    void AddObject(Object* object) { objects.push_back(object); }
    void AddLight(Light* light) { lights.push_back(light); }

    void loadOBJ(const std::string& path)
    {
        std::shared_ptr<Material> m = std::make_shared<Diffuse>();

        AddObject(Parser::loadOBJ(path, m, true));
    }
    
    inline std::vector<Object*> getObjects() const { return objects; }
    inline std::vector<Light*> getLights() const { return lights; }
    
    Hit intersect(const Ray& ray) const;
    
};