#pragma once
#include "GameEvent.hpp"
#include <vector>

class EventSystem
{
public:
    EventSystem() = default;
    ~EventSystem() = default;

    void enqueue(const GameEvent& event);

    const std::vector<GameEvent>& getEvents() const;
    void clear();

private:
    std::vector<GameEvent> m_events;
};
