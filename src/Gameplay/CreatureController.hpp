#pragma once
#include "EventSystem/EventListener.hpp"
#include <memory>

class Creature;

class CreatureController : public EventListener
{
public:
    using Ptr = std::unique_ptr<CreatureController>;

    CreatureController() = default;
    CreatureController(Creature* cre) : m_cre(cre) {}
    ~CreatureController() = default;

    virtual void update() = 0;

protected:
    virtual void onEvent(const GameEvent& event) = 0;

protected:
    Creature* m_cre = nullptr;
};
