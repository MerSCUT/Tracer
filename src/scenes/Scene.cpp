#include"stat_render/scenes/Scene.h"

Hit Scene::intersect(const Ray& ray)
{
    Hit payload;
    for(auto& obj : objects)
    {
        if (Hit temp = obj->intersect(ray); 
            temp.intersected && temp.tmin < payload.tmin)
        {
            payload = temp;
        }
    }
    return payload;
}