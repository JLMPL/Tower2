#include "EventSystem.hpp"
#include "EventListener.hpp"

EventSystem g_EventSys;

void EventSystem::addListener(EventListener* listener)
{
    m_listeners.push_back(listener);
}

void EventSystem::enqueue(const GameEvent& event)
{
    m_events.push_back(event);
}

void EventSystem::redistributeEvents()
{
    for (auto& listener : m_listeners)
    {
        for (auto& event : m_events)
        {
            if (event.isInCategory(listener->getInterest()))
                listener->onEvent(event);
        }
    }

    m_events.clear();
}

