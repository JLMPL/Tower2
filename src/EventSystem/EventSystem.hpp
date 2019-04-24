#pragma once
#include "GameEvent.hpp"
#include <vector>

class EventListener;

class EventSystem
{
public:
    EventSystem() = default;
    ~EventSystem() = default;

    void addListener(EventListener* listener);
    void enqueue(const GameEvent& event);

    void redistributeEvents();

private:
    std::vector<EventListener*> m_listeners;
    std::vector<GameEvent> m_events;
};

extern EventSystem g_EventSys;
