#include "Creature.hpp"
#include "Animation/Animation.hpp"
#include "Debug/DebugMenu.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/MeshManager.hpp"
#include "Render/Scene/RenderSkinnedMesh.hpp"
#include "Gameplay/Level.hpp"

Creature::Creature(u32 id, LevelContext* context, Species spec) :
    Entity(id, context), m_species(spec)
{
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
    // initLabel("Player", 2.25);

    m_animator = anim::addAnimatorToAnimSystem(&gfx::g_MeshMgr.getSkinnedMesh("rigud.dae")->skeleton, "Rigud");
    m_meshNode = m_context->renderScene->addRenderSkinnedMesh("rigud.dae", m_animator->matrixPalette.data());

    m_conto.init(m_context->physSys, &m_id, 0.25, 1.5);

    debug::g_Menu["Gameplay"]["Player"].bind("health", &m_health);
    debug::g_Menu["Gameplay"]["Player"].bind("maxHealth", (i32*)&m_maxHealth);
    debug::g_Menu["Gameplay"]["Player"].bind("magicka", &m_magicka);
    debug::g_Menu["Gameplay"]["Player"].bind("maxMagicka", (i32*)&m_maxMagicka);

    m_sword = 0;
}

void Creature::initSkeleton()
{
    // initLabel("Skeleton", 2.25);

    m_animator = anim::addAnimatorToAnimSystem(&gfx::g_MeshMgr.getSkinnedMesh("rigud.dae")->skeleton, "Rigud");
    m_meshNode = m_context->renderScene->addRenderSkinnedMesh("rigud.dae", m_animator->matrixPalette.data());

    m_conto.init(m_context->physSys, &m_id, 0.25, 1.5);
    // m_eq.init(m_id);
}

void Creature::update()
{
}

void Creature::lateUpdate()
{
    m_pos = m_conto.getFootPosition();

    m_yaw = atan2(m_facingDir.x, m_facingDir.z);
    m_transform = math::translate(m_pos) * math::rotate(m_yaw, math::vecY);

    // m_meshNode->setPosition(m_pos);
    // m_meshNode->setRotation(math::rotate(quat(), m_yaw, math::vecY));

    m_meshNode->setTransform(m_transform);

    // gfx::g_Renderer3D.addLine(m_pos + vec3(0,0.1,0), vec3(0,0.1,0) + m_pos + m_facingDir, vec3(0,1,0));
    // gfx::g_Renderer3D.addLine(m_pos + vec3(0,0.1,0), vec3(0,0.1,0) + m_pos + m_dir, vec3(0,0,1));
}

void Creature::move(Seconds deltaTime)
{
    m_conto.move(m_dir * m_speed, deltaTime);
}

void Creature::drawSword()
{
    m_swordDrawn = true;
}

void Creature::sheatheSword()
{
    m_swordDrawn = false;
}

bool Creature::isSwordDrawn()
{
    return m_swordDrawn;
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

    if (m_health > m_maxHealth)
        m_health = m_maxHealth;
}

void Creature::damage(i32 damage)
{
    m_health -= damage;

    if (m_health <= 0)
    {
        m_health = 0;
        m_isDead = true;
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

// void Creature::interact(Creature* other)
// {
// }

void Creature::setPos(const vec3& pos)
{
    m_pos = pos;
    m_conto.setPosition(pos + vec3(0,1,0));
}

Entity::Type Creature::getType() const
{
    return Type::Creature;
}

Creature::Species Creature::getSpecies() const
{
    return m_species;
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
    return *m_animator;
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
