#pragma once
#include<Eigen/Core>
#include<Eigen/Geometry>
#include<string>

using Vector3f = Eigen::Vector3f;        // invariant under translation
using Point2f = Eigen::Vector2f;
using Point3f = Eigen::Vector3f;

using Color3f = Eigen::Array<float, 3, 1>;

// 封装
inline float Dot(Vector3f v1, Vector3f v2) { return v1.dot(v2); }
inline Vector3f Cross(Vector3f v1, Vector3f v2) {return v1.cross(v2); }

inline constexpr float Inf = std::numeric_limits<float>::infinity();
inline constexpr float Pi = 3.1415926535897932384626;
inline constexpr float inv_Pi = 1./Pi;
inline constexpr float Epsilon = 1e-5f;

inline constexpr float deg2rad(float deg) { return deg * Pi / 180.0f; }
// 球面坐标映射到3D方向向量
inline Vector3f SphTo3D(float theta, float phi)
{
    return Vector3f(
        std::sin(theta)*std::cos(phi),
        std::sin(theta)*std::sin(phi),
        std::cos(theta)
    );
}
inline std::string output_path = "../images/output.ppm";