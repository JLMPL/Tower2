#pragma once
#include "Core/Math3D.hpp"

enum Button
{
    K_A = 0,
    K_D,
    K_E,
    K_S,
    K_W,
    K_F,

    K_Tab,
    K_LAlt,

    K_Space,

    K_F1,
    K_F2,
    K_F3,
    K_F4,

    K_1,
    K_2,

    M_Left,
    M_Right,

    Count
};

struct Event
{
    enum Type
    {
        Invalid = 0,
        ButtonPressed,
        AxisMoved
    };

    Type type;

    vec2 leftAxis;
    vec2 rightAxis;
    u32 button = 0u;

    Event() = default;
    Event(const Type& tpe) :
        type(tpe)
    {}
    Event(const Type& tpe, const vec2& left, const vec2& right) :
        type(tpe), leftAxis(left), rightAxis(right)
    {}
    Event(const Type& tpe, u32 butt) :
        type(tpe), button(butt)
    {}
};
