#include "CreatureController.hpp"

class PlayerController : public CreatureController
{
public:
    enum class State
    {
        Idle,
        Move,
        Dead,
        Attack,
        Pain,
        Jump
    };

    PlayerController() = default;
    PlayerController(Creature* cre);
    ~PlayerController() = default;

    void update() override final;

private:
    void onEvent(const GameEvent& event) override final;

    void enterIdle();
    void idle();

    void enterMove();
    void move();

    void enterAttack();
    void attack();

    void fall();

    void checkDrawWeapon();
    bool isAnyMovementKey() const;

private:
    State m_state = State::Idle;
};
