#include "GameEvent.hpp"

void GameEvent::setSender(i32 id)
{
    m_sender = id;
}

void GameEvent::setReceiver(i32 id)
{
    m_receiver = id;
}

i32 GameEvent::getSender() const
{
    return m_sender;
}

i32 GameEvent::getReceiver() const
{
    return m_receiver;
}

GameEvent::Type GameEvent::getType() const
{
    return m_type;
}
