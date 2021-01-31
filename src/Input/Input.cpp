#include "Input.hpp"
#include <SDL2/SDL.h>

Input gInput;

void Input::init()
{

}

void Input::update()
{
    reset();

    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_W])
    {
        m_leftAxis.y = 1.f;
    }
    if (keys[SDL_SCANCODE_S])
    {
        m_leftAxis.y = -1.f;
    }

    if (keys[SDL_SCANCODE_A])
    {
        m_leftAxis.x = -1.f;
    }
    if (keys[SDL_SCANCODE_D])
    {
        m_leftAxis.x = 1.f;
    }

    if (m_leftAxis != vec2(0))
    {
        m_leftAxis = math::normalize(m_leftAxis);
    }

    m_buttons[Button::K_E]     = keys[SDL_SCANCODE_E];
    m_buttons[Button::K_LAlt]  = keys[SDL_SCANCODE_LALT];
    m_buttons[Button::K_Space] = keys[SDL_SCANCODE_SPACE];
    m_buttons[Button::K_Tab]   = keys[SDL_SCANCODE_TAB];
    m_buttons[Button::K_1]     = keys[SDL_SCANCODE_1];
    m_buttons[Button::K_2]     = keys[SDL_SCANCODE_2];

    i32 mx, my;
    SDL_GetRelativeMouseState(&mx, &my);
    m_rightAxis = vec2(f32(mx), f32(my));

    u32 buttons = SDL_GetMouseState(nullptr, nullptr);
    m_buttons[Button::M_Left] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    m_buttons[Button::M_Right] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
}

vec2 Input::getLeftAxis() const
{
    return m_leftAxis;
}

vec2 Input::getRightAxis() const
{
    return m_rightAxis;
}

bool Input::isUse() const
{
    return m_buttons[Button::K_E];
}

bool Input::isFocus() const
{
    return m_buttons[Button::M_Right];
}

bool Input::isAttack() const
{
    return m_buttons[Button::M_Left];
}

bool Input::isDrawMelee() const
{
    return m_buttons[Button::K_1];
}

bool Input::isJump() const
{
    return m_buttons[Button::K_LAlt];
}

bool Input::isButton(const Button& button) const
{
    return m_buttons[static_cast<u32>(button)];
}

void Input::reset()
{
    m_leftAxis = vec2(0);
    m_rightAxis = vec2(0);

    for (u32 i = 0; i < static_cast<u32>(Button::Count); i++)
        m_buttons[i] = false;
}
