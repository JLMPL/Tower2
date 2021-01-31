#include "Timer.hpp"

namespace timer
{

float delta = 0;

}

namespace core
{

Seconds resetTimer(Timer& timer)
{
    timer.now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<f32, std::ratio<1>> diff = timer.now - timer.then;

    timer.then = std::chrono::high_resolution_clock::now();

    return diff.count();
}

Seconds getElapsedTime(Timer& timer)
{
    timer.now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<f32, std::ratio<1>> diff = timer.now - timer.then;
    return diff.count();
}

}
