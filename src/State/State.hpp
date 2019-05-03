#pragma once
#include "Core/Types.hpp"
#include "Events/Event.hpp"
#include <memory>

struct Event;
class Renderer2D;
class Resources;
class StateStack;

class State
{
public:
    using Ptr = std::unique_ptr<State>;

    enum Type
    {
        Menu,
        Playing,
        UI,
        Debug,
        Loading,

        Count
    };

    State() = default;
    State(StateStack& ss) : m_stateStack(ss) {}
    virtual ~State() = default;

    virtual void init() = 0;
    virtual void sendSystemEvent(const Event& event) = 0;
    virtual void update() = 0;
    virtual void draw() {}
    virtual void drawGui() {}

protected:
    StateStack& m_stateStack;
};
