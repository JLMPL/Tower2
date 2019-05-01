#pragma once
#include "Core/Json.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "AI/Waynet.hpp"
#include "Interactible.hpp"
#include "Projectile.hpp"
#include "Creature.hpp"

#include "SceneGraph/SceneGraph.hpp"

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
    void loload(const std::string& file);
    void update();
    void lateUpdate();

    void sendSystemEvent(const SDL_Event& event);

    Creature* addCreature(Creature::Species species);
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
    void setLevelMesh(const std::string& map, const std::string& net);

    void createEntities();
    void destroyEntities();

    void createProjectiles();
    void destroyProjectiles();

private:
    std::string                  m_name;
    Waynet                       m_waynet;

    SceneGraph m_sceneGraph;
    SceneNode* m_mapMesh = nullptr;

    u32                          m_lastEntityId = 0;
    std::vector<Entity::Ptr>     m_creationQueue;
    std::vector<Entity::Ptr>     m_entities;

    std::vector<Projectile::Ptr> m_projectileQueue;
    std::vector<Projectile::Ptr> m_projectiles;
};
