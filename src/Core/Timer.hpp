#pragma once
#include "Types.hpp"

namespace core
{

class Timer
{
public:
    Timer();

    Seconds reset();
    Seconds getElapsedTime() const;

private:
    Moment m_then;
    mutable Moment m_now;
};

}
