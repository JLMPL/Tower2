#include "EventListener.hpp"

void EventListener::initEventListening(GameEvent::Category cat)
{
    m_interest = cat;
}

GameEvent::Category EventListener::getInterest() const
{
    return m_interest;
}
