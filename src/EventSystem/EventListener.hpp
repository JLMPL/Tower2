#pragma once
#include "GameEvent.hpp"

class EventListener
{
public:
    EventListener() = default;
    virtual ~EventListener() = default;

    void initEventListening(GameEvent::Category cat);

    virtual void onEvent(const GameEvent& event) = 0;

    GameEvent::Category getInterest() const;

protected:
    GameEvent::Category m_interest = GameEvent::Category::Null;
};
