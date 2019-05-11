#include "EventSystem.hpp"

void EventSystem::enqueue(const GameEvent& event)
{
    m_events.push_back(event);
}

const std::vector<GameEvent>& EventSystem::getEvents() const
{
    return m_events;
}

void EventSystem::clear()
{
    m_events.clear();
}
