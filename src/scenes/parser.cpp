#include"stat_render/scenes/parser.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // for std::replace


// 辅助函数：从 "v/vt/vn" 格式的字符串中提取顶点索引
// 例如输入 "12/5/1"，返回 12
int get_vertex_index(std::string str) {
    size_t slash_pos = str.find('/');
    if (slash_pos != std::string::npos) {
        return std::stoi(str.substr(0, slash_pos));
    }
    return std::stoi(str);
}

Mesh* Parser::loadOBJ(const std::string& filename, std::shared_ptr<Material> material) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return nullptr;
    }

    std::vector<Vector3f> vertices;
    std::vector<uint32_t> indices;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            // 解析顶点位置: v x y z
            float x, y, z;
            ss >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        } 
        else if (prefix == "f") {
            // 解析面索引: f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
            // 由于保证是三角形，所以直接读取3组数据
            std::string s1, s2, s3;
            ss >> s1 >> s2 >> s3;

            // 提取顶点索引并转为 0-based
            // OBJ 文件的索引是从 1 开始的，所以必须 -1
            indices.push_back(get_vertex_index(s1) - 1);
            indices.push_back(get_vertex_index(s2) - 1);
            indices.push_back(get_vertex_index(s3) - 1);
        }
        // 这里暂时忽略 vt (纹理坐标) 和 vn (法线)，后续可以扩展
    }

    file.close();

    std::cout << "Loaded Mesh: " << filename 
              << " | Vertices: " << vertices.size() 
              << " | Triangles: " << indices.size() / 3 << std::endl;
    

    // 利用之前定义的 Mesh 构造函数创建对象
    return new Mesh(vertices, indices, material.get());
}