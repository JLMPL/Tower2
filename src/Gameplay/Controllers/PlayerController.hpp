#include "Gameplay/CreatureController.hpp"
#include "Core/Math3D.hpp"
#include "Core/Timer.hpp"
#include "Physics/Cloth.hpp"
#include "Physics/RigidBody.hpp"

class RenderFlare;
class RenderMesh;
class RenderCloth;

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
    void preSimulationUpdate() override final;

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

    i32 m_combatTarget = -1;

    vec3 m_moveTowardsDir = vec3(1,0,0);

    RenderMesh* m_sord = nullptr;
    RenderFlare* m_light = nullptr;
    RenderCloth* m_capeNode = nullptr;
    RenderCloth* m_capeNode2 = nullptr;

    phys::Cloth* m_cape = nullptr;
    phys::Cloth* m_cape2 = nullptr;
    phys::Cloth::Sphere m_spheres[6];

    phys::RigidBody m_dongle;

    f32 m_verticalVelocity = 0;

    core::Timer m_lolo;

    i32 m_spawnTimer = 0;

    i32 m_combo = 0;
};
