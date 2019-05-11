#pragma once
#include <memory>

class Creature;
struct GameEvent;

class CreatureController
{
public:
    using Ptr = std::unique_ptr<CreatureController>;

    CreatureController() = default;
    CreatureController(Creature* cre) : m_cre(cre) {}
    ~CreatureController() = default;

    virtual void update() = 0;
    virtual void onEvent(const GameEvent& event) = 0;

protected:

protected:
    Creature* m_cre = nullptr;
};
