#pragma once
#include "Core/Math3D.hpp"

namespace gfx
{
    class StaticMesh;
};

namespace ecs::comp
{

enum class TransformComputation
{
    UseMatrix,
    MultiplyPosRot
};

struct Transform
{
    static constexpr u32 Signature = SET_BIT(0);

    TransformComputation type;

    mat4 tr;
    vec3 pos;
    quat rot;
    vec3 dir;
};

struct StaticMesh
{
    static constexpr u32 Signature = SET_BIT(1);

    gfx::StaticMesh* mesh = nullptr;
};

}
