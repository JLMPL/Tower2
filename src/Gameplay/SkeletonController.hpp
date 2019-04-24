#include "CreatureController.hpp"

class SkeletonController : public CreatureController
{
public:
    enum class State
    {
        Idle,
        Dead,
        Attack,
        Pain,
    };

    SkeletonController() = default;
    SkeletonController(Creature* cre);
    ~SkeletonController() = default;

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
