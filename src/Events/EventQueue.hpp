#pragma once
#include "Event.hpp"

template <typename T>
class EventQueue
{
public:
    EventQueue() = default;
    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;

    void enqueue(const T& event)
    {
        m_queue[m_numEvent++] = event;
    }

    bool getNextEvent(T& event)
    {
        bool is = false;

        if (m_numEvent > 0)
        {
            if (m_eventIndex < m_numEvent)
            {
                event = m_queue[m_eventIndex];
                is = true;
                m_eventIndex++;
            }
            else
            {
                m_numEvent = 0;
                m_eventIndex = 0;
            }
        }

        return is;
    }

private:
    static constexpr u32 MaxEvents = 128u;

    T m_queue[MaxEvents];

    u32 m_numEvent = 0;
    u32 m_eventIndex = 0;
};
