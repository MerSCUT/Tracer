#pragma once

#include"stat_render/core/common.h"
#include"stat_render/shapes/Mesh.h"
class Parser
{
private:
public:
    static bool loadOBJ(const std::string& path, Mesh& mesh);
    static Mesh* loadOBJ(const std::string& filename, std::shared_ptr<Material> material);
};