#include"stat_render/acclerators/Bound.h"

Bound Bound::Union(const Bound& b1)
{
    Bound b2 = *this;
    Vector3f pmin = (b1.Pmin.array() < b2.Pmin.array()).select(b1.Pmin, b2.Pmin);
    Vector3f pmax = (b1.Pmax.array() > b2.Pmax.array()).select(b1.Pmax,b2.Pmax);
    return Bound(pmin, pmax);
}