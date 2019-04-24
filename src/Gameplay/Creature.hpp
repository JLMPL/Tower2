#pragma once
#include "Physics/CharacterController.hpp"
#include "Animation/AnimationSystem.hpp"
#include "Core/Timer.hpp"
#include "Interactible.hpp"
#include "Equipment.hpp"
#include "CreatureController.hpp"

class Level;
class Item;
class SceneGraph;
class SceneNode;

class Creature : public Interactible
{
public:
    using Base = Entity;

    enum class DrawnWeapon
    {
        None = 0,
        Sword,
        Bow
    };

    enum class Species
    {
        Player = 0,
        Skeleton,
        Rat,
        Bat,
        Goblin,

        Count
    };

    void                 init(u32 id, Level* level, SceneGraph* graph, Species spec);

    void                 setController(CreatureController* ctrl);

    void                 update() override final;
    void                 lateUpdate() override final;

    void                 onEvent(const GameEvent& event) override final {}

    void                 interact(Creature* other) override final;
    void                 move(Seconds deltaTime = 1000_ms);

    u32                  getDamageOutput() const;
    u32                  getDefense() const;

    void                 heal(i32 heal);
    void                 damage(Creature* other, i32 damage);

    void                 drawSword();
    void                 sheatheSword();
    bool                 isSwordDrawn();

    void                 drawBow();
    void                 sheatheBow();
    bool                 isBowDrawn();

    u32                  getMaxHealth() const;
    u32                  getMaxMagicka() const;
    i32                  getHealth() const;
    i32                  getMagicka() const;

    Level*               getLevel();
    Interactible*        getFocus();
    Equipment*           getEquipment();
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

    bool                 isPlayer() const;

    void                 setBusy(bool busy);
    bool                 isBusy() const;

    void                 kill();
    bool                 isDead() const;

private:
    void                 initPlayer();
    void                 initSkeleton();

private:
    Species                 m_species = Species::Count;
    Level*                  m_level = nullptr;

    CreatureController::Ptr m_controller;

    SceneGraph*             m_sceneGraph = nullptr;
    SceneNode*              m_meshNode = nullptr;

    phys::CharacterController m_conto;

    u32                     m_maxHealth = 10;
    u32                     m_maxMagicka = 10;

    i32                     m_health = 5;
    i32                     m_magicka = 5;

    bool                    m_isPlayer = false;
    bool                    m_isDead = false;

    bool                    m_busy = false;

    vec3                    m_dir = {0,0,1};
    vec3                    m_facingDir = {0,0,1};
    f32                     m_speed = 5.f;
    f32                     m_yaw = 0.f;

    Interactible*           m_focus = nullptr;
    Equipment               m_eq;

    std::vector<i32> m_fuck;

    DrawnWeapon             m_drawnWeapon = DrawnWeapon::None;
    i32                     m_sword = -1;
    i32                     m_bow = -1;
};
