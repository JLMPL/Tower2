#pragma once
#include "Core/Math3D.hpp"
#include "Animation/Animation.hpp"

struct SkinningData
{
    std::vector<mat4> palette;
    std::vector<mat4> transforms;

    void computeMatrices(const Skeleton& skeleton, const Pose& pose);
};