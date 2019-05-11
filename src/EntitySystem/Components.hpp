#pragma once
#include "Core/Math3D.hpp"
#include <string>

namespace gfx
{
    class StaticMesh;
};

class MeshNode;

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

    MeshNode* meshNode = nullptr;

    StaticMesh() = default;
    StaticMesh(const std::string& name);
};

}
