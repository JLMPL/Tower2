#pragma once
#include "Core/Math3D.hpp"

namespace comp
{

struct Transform
{
    mat4 mat;
};

struct StaticMesh
{
    RenderMesh* meshNode = nullptr;
    mat4 offset;
};

struct SkinnedMesh
{
    RenderSkinnedMesh* skinNode = nullptr;
    AnimationHandle anim;

    float speed = 1.f;

    Skeleton skeleton;
    Pose pose;
    SkinningData skinData;

    float along = 0.f;
};

struct Camera
{
    vec3 target;
    vec3 eye;
    mat4 view;
};

}