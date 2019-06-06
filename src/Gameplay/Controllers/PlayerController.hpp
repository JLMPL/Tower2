#include "Gameplay/CreatureController.hpp"
#include "Core/Math3D.hpp"
#include "Core/Timer.hpp"

namespace phys
{
    class Cloth;
}

class SceneNode;
class SceneGraph;
class LightNode;
class FlareNode;
class MeshNode;
class CameraNode;
class ClothNode;
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
    void onEvent(const GameEvent& event) override final;

private:

    void enterIdle();
    void idle();

    void enterMove();
    void move();

    void enterAttack();
    void attack();

    void fall();

    void moveCamera();

    void checkDrawWeapon();
    bool isAnyMovementKey() const;

    void updateHud();

private:
    State m_state = State::Idle;

    LevelContext* m_context = nullptr;

    Interactible* m_interactible = nullptr;
    i32 m_combatTarget = -1;

    CameraNode* m_camera = nullptr;
    SceneNode* m_cameraHolder = nullptr;
    vec3 m_moveTowardsDir = vec3(1,0,0);
    f32 m_cameraHolderYaw = 0.f;
    f32 m_cameraHolderPitch = 0.f;

    MeshNode* m_sord = nullptr;
    FlareNode* m_light = nullptr;
    ClothNode* m_capeNode = nullptr;

    phys::Cloth* m_cape = nullptr;



    core::Timer m_lolo;

    i32 m_spawnTimer = 0;

    i32 m_combo = 0;
};
