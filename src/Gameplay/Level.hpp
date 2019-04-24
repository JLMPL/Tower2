#pragma once
#include "Core/Json.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "AI/Waynet.hpp"
#include "Render/OrbitCamera.hpp"
#include "CameraController.hpp"
#include "Interactible.hpp"
#include "Projectile.hpp"

#include "SceneGraph/SceneGraph.hpp"

#include "Render/Light.hpp"

union SDL_Event;

class Creature;
class Pickup;
class Chest;
class Door;
class Lever;
class Animator;

class Level
{
public:
    Level() = default;
    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;

    void loadFromFile(const Path& path);
    void update();
    void lateUpdate();

    void sendSystemEvent(const SDL_Event& event);

    Creature* addCreature(const std::string& type, bool isPlayer = false);
    Pickup*   addPickup(const std::string& item);
    Chest*    addChest(const Code& code);
    Door*     addDoor(const std::string& code);
    Lever*    addLever(const std::string& code);

    Projectile* addProjectile(Projectile* proj);

    Waynet& getWaynet();

    Interactible* getClosestInteractible(const vec3& pos, const vec3& dir);
    Creature*     getClosestCombatTarget(const vec3& pos, const vec3& dir, bool onlyPlayer = false);
    Entity*       getEntityById(u32 id) const;

private:
    void loadMap(const std::string& map, const std::string& net);
    void loadEntities(const json& json);
    void loadCreature(const json& json);
    void loadChest(const json& json);
    void loadDoor(const json& json);
    void loadLever(const json& json);

    void createEntities();
    void destroyEntities();

    void createProjectiles();
    void destroyProjectiles();

private:
    std::string                  m_name;
    Waynet                       m_waynet;

    SceneGraph m_sceneGraph;
    SceneNode* m_mapMesh = nullptr;

    CameraController             m_cameraController;

    u32                          m_lastEntityId = 0;
    std::vector<Entity::Ptr>     m_creationQueue;
    std::vector<Entity::Ptr>     m_entities;

    std::vector<Projectile::Ptr> m_projectileQueue;
    std::vector<Projectile::Ptr> m_projectiles;
};
