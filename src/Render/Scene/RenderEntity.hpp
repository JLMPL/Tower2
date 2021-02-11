#pragma once
#include "Core/Math3D.hpp"
#include <vector>
#include <memory>
#include "Core/NeatCast.hpp"

struct RenderEntity
{
    using Ptr = std::unique_ptr<RenderEntity>;
    using Ref = std::shared_ptr<RenderEntity>;

    NEAT_CAST;

    mat4 transform;
    bool visible = true;
};
