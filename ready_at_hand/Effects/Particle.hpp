#pragma once
#include "Core/Math3D.hpp"

struct Particle
{
    bool isUsed = false;

    vec3 pos;
    vec3 vel;
    f32 weight = 1;
    f32 lifetime = 1;
    f32 timer = 0;
};
