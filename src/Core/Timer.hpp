#pragma once
#include "Types.hpp"

namespace core
{

struct Timer
{
    Moment then = std::chrono::high_resolution_clock::now();
    Moment now = std::chrono::high_resolution_clock::now();
};

Seconds resetTimer(Timer& timer);
Seconds getElapsedTime(Timer& timer);

}
