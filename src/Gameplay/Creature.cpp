#include "Creature.hpp"
#include "Animation/AnimationSystem.hpp"
#include "Debug/DebugMenu.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Interface/Interface.hpp"
#include "Interface/Interface.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/MeshManager.hpp"
#include "SceneGraph/SkinnedMeshNode.hpp"
#include "SceneGraph/LightNode.hpp"
#include "ItemManager.hpp"
#include "Level.hpp"
#include "Pickup.hpp"

Creature::Creature(u32 id, LevelContext* context) : Interactible(id, context)
{
}

void Creature::init(Species spec)
{
    m_species = spec;

    switch (m_species)
    {
        case Species::Player:
            initPlayer();
            break;
        case Species::Skeleton:
            initSkeleton();
            break;
    }
}

void Creature::initPlayer()
{
    initLabel("Player", 2.25);

    m_meshNode = m_context->sceneGraph->addSkinnedMeshNode("humx.dae", "Human");

    // auto camra = m_context->sceneGraph->addCameraNode();
    // m_meshNode->attachNode(camra);

    m_context->sceneGraph->getRoot()->attachNode(m_meshNode);

/*    auto light = m_context->sceneGraph->addLightNode();
    auto li = static_cast<LightNode*>(light);
    li->setColor(vec3(1,0.75,0.01) * 100);
    li->setShadowCasting(true);
    li->setPosition(vec3(0,2,0));

    m_meshNode->attachNode(light);*/

    m_conto.init(m_context->physSys, &m_id, 0.25, 1.5);
    // m_eq.init(m_id);

    debug::g_Menu["Gameplay"]["Player"].bind("health", &m_health);
    debug::g_Menu["Gameplay"]["Player"].bind("maxHealth", (i32*)&m_maxHealth);
    debug::g_Menu["Gameplay"]["Player"].bind("magicka", &m_magicka);
    debug::g_Menu["Gameplay"]["Player"].bind("maxMagicka", (i32*)&m_maxMagicka);

    // m_sword = g_ItemMgr.getItem("damn_sord")->m_id;
    m_sword = 0;

    // ui::g_Interface.setWeapons(m_fuck);
}

void Creature::initSkeleton()
{
    initLabel("Skeleton", 2.25);

    auto mesh = gfx::g_MeshMgr.getSkinnedMesh("humx.dae");

    m_meshNode = m_context->sceneGraph->addSkinnedMeshNode("humx.dae", "Human");
    m_context->sceneGraph->getRoot()->attachNode(m_meshNode);

    m_conto.init(m_context->physSys, &m_id, 0.25, 1.5);
    // m_eq.init(m_id);
}

void Creature::setController(CreatureController* ctrl)
{
    m_controller = CreatureController::Ptr(ctrl);
}

void Creature::update()
{
    if (m_controller)
        m_controller->update();
}

void Creature::lateUpdate()
{
    m_pos = m_conto.getFootPosition();

    m_yaw = atan2(m_facingDir.x, m_facingDir.z);
    m_transform = math::translate(m_pos) * math::rotate(m_yaw, math::vecY);

    m_meshNode->setPosition(m_pos);
    m_meshNode->setRotation(math::rotate(quat(), m_yaw, math::vecY));

    // gfx::g_Renderer3D.addLine(m_pos + vec3(0,0.1,0), vec3(0,0.1,0) + m_pos + m_facingDir, vec3(0,1,0));
    // gfx::g_Renderer3D.addLine(m_pos + vec3(0,0.1,0), vec3(0,0.1,0) + m_pos + m_dir, vec3(0,0,1));
}

void Creature::move(Seconds deltaTime)
{
    m_conto.move(m_dir * m_speed, deltaTime);
}

void Creature::drawSword()
{
    m_drawnWeapon = DrawnWeapon::Sword;
}

void Creature::sheatheSword()
{
    m_drawnWeapon = DrawnWeapon::None;
}

bool Creature::isSwordDrawn()
{
    return m_drawnWeapon == DrawnWeapon::Sword;
}


void Creature::drawBow()
{

}

void Creature::sheatheBow()
{

}

bool Creature::isBowDrawn()
{
    return m_drawnWeapon == DrawnWeapon::Bow;
}


u32 Creature::getDamageOutput() const
{
    return 1;
}

u32 Creature::getDefense() const
{
    return 1;
}

void Creature::heal(i32 heal)
{
    m_health += heal;
}

void Creature::damage(Creature* other, i32 damage)
{
    if (isDead())
        return;

    m_health -= damage;

    if (m_health <= 0)
    {
        m_health = 0;
        kill();

        if (!m_isPlayer)
        {
            // auto pick = getLevel()->addPickup("damn_herb");
            // pick->addRigidBody(getPos() + vec3(0.1,3,0.1));
        }
    }
}

u32 Creature::getMaxHealth() const
{
    return m_maxHealth;
}

u32 Creature::getMaxMagicka() const
{
    return m_maxMagicka;
}

i32 Creature::getHealth() const
{
    return m_health;
}

i32 Creature::getMagicka() const
{
    return m_magicka;
}

void Creature::interact(Creature* other)
{
}

void Creature::setPos(const vec3& pos)
{
    m_pos = pos;
    m_conto.setPosition(pos + vec3(0,1,0));
}

Entity::Type Creature::getType() const
{
    return Type::Creature;
}

bool Creature::isPlayer() const
{
    return m_isPlayer;
}

void Creature::setDirection(const vec3& dir)
{
    m_dir = dir;
}

vec3 Creature::getDirection() const
{
    return m_dir;
}

void Creature::setFacingDirection(const vec3& dir)
{
    m_facingDir = dir;
}

vec3 Creature::getFacingDir() const
{
    return m_facingDir;
}

f32 Creature::getYaw() const
{
    return m_yaw;
}

anim::Animator& Creature::getAnimator()
{
    return *static_cast<SkinnedMeshNode*>(m_meshNode)->getAnimator();
}

phys::CharacterController& Creature::getCharCtrl()
{
    return m_conto;
}

void Creature::kill()
{
    m_isDead = true;
}

bool Creature::isDead() const
{
    return m_isDead;
}
