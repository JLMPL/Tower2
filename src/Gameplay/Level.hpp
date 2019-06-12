#pragma once
#include "AI/Waynet.hpp"
#include "Animation/AnimationSystem.hpp"
#include "Controllers/CameraController.hpp"
#include "CreatureController.hpp"
#include "Entities/Camera.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Gameplay/Entities/Creature.hpp"
#include "Interface/HeadsUpDisplay.hpp"
#include "Render/Scene/RenderScene.hpp"
#include "Script/Lua.hpp"

union SDL_Event;

class Interactible;
class Pickup;
class Chest;
class Door;
class Lever;
class Animator;

struct LevelContext
{
    class Level*           level = nullptr;
    RenderScene*           renderScene = nullptr;
    EventSystem*           eventSys = nullptr;
    anim::AnimationSystem* animSys = nullptr;
    phys::PhysicsSystem*   physSys = nullptr;
    Camera*                camera = nullptr;
};

class Level
{
public:
    Level() = default;
    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;

    void initFromScript(const std::string& file);
    void uploadFunctions(lua::state& state);
    void onEvent(const GameEvent& event);
    void update();
    void draw();

    void sendSystemEvent(const SDL_Event& event);

    u32 addCreature(Creature::Species species, const vec3& pos = vec3(0));
    u32 addPickup(u32 item, const vec3& pos = vec3(0));
    u32 addChest(const vec3& pos = vec3(0));
    u32 addDoor(const vec3& pos = vec3(0));
    u32 addLever(u32 target, const vec3& pos = vec3(0));

    void addLightEffect(vec3 pos);

    Waynet& getWaynet();

    Entity* getEntityByID(u32 id);

    Interactible* getClosestInteractible(const vec3& pos, const vec3& dir);
    Creature*     getClosestCombatTarget(const vec3& pos, const vec3& dir, bool onlyPlayer = false);

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
    EventSystem m_eventSys;
    anim::AnimationSystem m_animSys;
    phys::PhysicsSystem m_physSys;

    ui::HeadsUpDisplay m_hud;

    u32                          m_lastEntityId = 0;
    std::vector<Entity::Ptr>     m_creationQueue;
    std::vector<Entity::Ptr>     m_entities;

    Camera m_camera;
    CameraController m_cameraCtrl;

    std::vector<CreatureController::Ptr> m_controllers;
};
