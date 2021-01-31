#pragma once
#include "AI/Waynet.hpp"
#include "Animation/Animation.hpp"
#include "Controllers/CameraController.hpp"
#include "CreatureController.hpp"
#include "Entities/Camera.hpp"
#include "Gameplay/Entities/Creature.hpp"
#include "Interface/HeadsUpDisplay.hpp"
#include "Render/Effects/ParticleSystem.hpp"
#include "Render/Scene/RenderScene.hpp"

union SDL_Event;

class Animator;

struct LevelContext
{
    class Level*           level = nullptr;
    RenderScene*           renderScene = nullptr;
    // EventSystem*           eventSys = nullptr;
    // anim::AnimationSystem* animSys = nullptr;
    phys::PhysicsSystem*   physSys = nullptr;
    ParticleSystem*        particleSys = nullptr;
    Camera*                camera = nullptr;
};

class Level
{
public:
    Level() = default;
    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;

    void initFromScript(const std::string& file);
    void onEvent(const GameEvent& event);
    void update();
    void draw();

    void sendSystemEvent(const SDL_Event& event);

    u32 addCreature(Creature::Species species, const vec3& pos = vec3(0));

    void addLightEffect(vec3 pos);

    Waynet& getWaynet();

    Entity* getEntityByID(u32 id);

private:
    void setLevelMesh(const std::string& map, const std::string& net);

    void createEntities();
    void destroyEntities();

private:
    std::string m_name;
    Waynet      m_waynet;

    RenderScene m_renderScene;
    RenderMesh* m_mapMesh = nullptr;

    LevelContext m_lvlContext;
    // anim::AnimationSystem m_animSys;
    phys::PhysicsSystem m_physSys;
    ParticleSystem m_particleSystem;

    // ParticleGroup* m_particleGroup = nullptr;
    // ParticleAffector* m_particleAffector = nullptr;
    // RenderParticles* m_renderParticles = nullptr;

    ui::HeadsUpDisplay m_hud;

    u32                          m_lastEntityId = 0;
    std::vector<Entity::Ptr>     m_creationQueue;
    std::vector<Entity::Ptr>     m_entities;

    Camera m_camera;
    CameraController m_cameraCtrl;

    std::vector<CreatureController::Ptr> m_controllers;
};
