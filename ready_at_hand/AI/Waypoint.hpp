#pragma once
#include "Core/Math3D.hpp"
#include <vector>

struct Waypoint
{
    static constexpr u32 MaxNeighbors = 8u;
    vec3 pos;

    f32 gCost = 0;
    f32 hCost = 0;
    f32 fCost = 0;

    Waypoint* neighbors[MaxNeighbors];
    u32 numNeighbors = 0;

    Waypoint* cameFrom = nullptr;

    void connect(Waypoint* other)
    {
        if (other)
        {
            if (numNeighbors < MaxNeighbors)
                neighbors[numNeighbors++] = other;

            if (other->numNeighbors < MaxNeighbors)
                other->neighbors[other->numNeighbors++] = this;
        }
    }
};
