#include "Timer.hpp"

namespace core
{

Timer::Timer()
{
    m_then = std::chrono::high_resolution_clock::now();
    m_now = std::chrono::high_resolution_clock::now();
}

Seconds Timer::reset()
{
    m_now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<f32, std::ratio<1>> diff = m_now - m_then;

    m_then = std::chrono::high_resolution_clock::now();

    return diff.count();
}

Seconds Timer::getElapsedTime() const
{
    m_now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<f32, std::ratio<1>> diff = m_now - m_then;
    return diff.count();
}

}
