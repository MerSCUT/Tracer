#pragma once

#include"stat_render/core/common.h"
#include<limits>
class Bound{
private:
    Point3f Pmin, Pmax;
public:
    Bound() : 
    Pmin(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max() ), 
    Pmax(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()) {}

    Bound(const Point3f& pmin_, const Point3f & pmax_) : Pmin(pmin_), Pmax(pmax_) {} 


    ~Bound() = default;
    // 包围盒合并函数
    Bound Union(const Bound& b1);

    Point3f Center() {return (Pmin + Pmax) / 2.f; }

    Point3f getPmin() {return Pmin; }
    Point3f getPmax() {return Pmax; }
    
    float MaxExtent() { 
        auto d = Pmax-Pmin;
        return std::max(d.x(), std::max(d.y(), d.z())); 
    }
};