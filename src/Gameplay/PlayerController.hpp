#include "CreatureController.hpp"
#include "Core/Math3D.hpp"

class SceneNode;
class SceneGraph;
class CameraNode;
class Interactible;

struct LevelContext;

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
    PlayerController(Creature* cre, LevelContext* context);
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

    // SceneGraph& m_sceneGraph;
    LevelContext* m_context = nullptr;

    Interactible* m_interactible = nullptr;

    CameraNode* m_camera = nullptr;
    SceneNode* m_cameraHolder = nullptr;
    vec3 m_moveTowardsDir = vec3(1,0,0);
    f32 m_cameraHolderYaw = 0.f;
};
