#pragma once
#include "Core/Math3D.hpp"
#include "Events/Event.hpp"

class Input
{
public:
    Input() = default;
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;
    ~Input() = default;

    void init();
    void update();

    vec2 getLeftAxis() const;
    vec2 getRightAxis() const;

    bool isUse() const;
    bool isFocus() const;
    bool isAttack() const;
    bool isDrawMelee() const;
    bool isDrawBow() const;
    bool isJump() const;

    bool isButton(const Button& button) const;

    void reset();

protected:
    bool m_buttons[static_cast<u32>(Button::Count)] = {false};

    vec2 m_leftAxis;
    vec2 m_rightAxis;
};

extern Input gInput;
