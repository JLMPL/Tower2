#pragma once
#include "Physics/CharacterController.hpp"
#include "Core/Timer.hpp"
#include "Interactible.hpp"

class Level;
class Item;
class RenderSkinnedMesh;

struct LevelContext;

class Creature : public Interactible
{
public:
    using Base = Entity;

    enum class Species
    {
        Player = 0,
        Skeleton,
        Rat,
        Bat,
        Goblin,

        Count
    };

    Creature(u32 id, LevelContext* context, Species spec);

    void                 update() override final;
    void                 lateUpdate() override final;

    void                 interact(Creature* other) override final;
    void                 move(Seconds deltaTime = 1000_ms);

    u32                  getDamageOutput() const;
    u32                  getDefense() const;

    void                 heal(i32 heal);
    void                 damage(i32 damage);

    void                 drawSword();
    void                 sheatheSword();
    bool                 isSwordDrawn();

    u32                  getMaxHealth() const;
    u32                  getMaxMagicka() const;
    i32                  getHealth() const;
    i32                  getMagicka() const;

    anim::Animator&      getAnimator();
    phys::CharacterController& getCharCtrl();

    void                 setDirection(const vec3& dir);
    vec3                 getDirection() const;
    void                 setFacingDirection(const vec3& dir);
    vec3                 getFacingDir() const;
    f32                  getYaw() const;
    bool                 shouldFaceEnemy() const;

    void                 setPos(const vec3& pos) override final;
    Type                 getType() const override final;
    Species              getSpecies() const;

    void                 kill();
    bool                 isDead() const;

private:
    void                 initPlayer();
    void                 initSkeleton();

private:
    Species                   m_species = Species::Count;
    RenderSkinnedMesh*        m_meshNode = nullptr;

    anim::Animator*           m_animator = nullptr;
    phys::CharacterController m_conto;

    u32                       m_maxHealth = 3;
    u32                       m_maxMagicka = 3;

    i32                       m_health = 3;
    i32                       m_magicka = 3;

    bool                      m_isDead = false;

    vec3                      m_dir = {0,0,1};
    vec3                      m_facingDir = {0,0,1};
    f32                       m_speed = 5.f;
    f32                       m_yaw = 0.f;

    bool                      m_swordDrawn = false;
    i32                       m_sword = -1;
};
