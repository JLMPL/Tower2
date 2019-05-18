#include "Gameplay/CreatureController.hpp"
#include "Core/Math3D.hpp"
#include "Core/Timer.hpp"

class SceneNode;
class SceneGraph;
class LightNode;
class FlareNode;
class MeshNode;
class CameraNode;
class Interactible;

struct LevelContext;

class SkeletonController : public CreatureController
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

    SkeletonController() = default;
    SkeletonController(Creature* cre, LevelContext* context);
    ~SkeletonController() = default;

    void update() override final;
    void onEvent(const GameEvent& event) override final;

private:

    void enterIdle();
    void idle();

    void enterMove();
    void move();

    void enterAttack();
    void attack();

    void enterPain();
    void pain();

    void fall();

    void moveCamera();

    void checkDrawWeapon();
    bool isAnyMovementKey() const;

private:
    State m_state = State::Idle;

    LevelContext* m_context = nullptr;

    i32 m_combatTarget = -1;

    vec3 m_moveTowardsDir = vec3(1,0,0);

    MeshNode* m_sord = nullptr;
    FlareNode* m_light = nullptr;

    i32 m_combo = 0;
};
