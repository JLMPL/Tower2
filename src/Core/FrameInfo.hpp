#pragma once
#include "Types.hpp"

namespace core
{

struct FrameInfo
{
    f64 globalTime = 0.0;
    u64 globalFrame = 0;

    f32 delta = 0.f;
};

extern FrameInfo g_FInfo;

}
