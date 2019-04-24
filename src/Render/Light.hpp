#pragma once
#include "Core/Math3D.hpp"
#include <memory>

namespace gfx
{

struct Light
{
    using Ptr = std::unique_ptr<Light>;

    vec3 pos;
    vec3 color;
    f32 intensity = 10.f;
    bool enabled = true;
};

}
