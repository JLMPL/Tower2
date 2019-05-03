#include "Level.hpp"
#include "Debug/Switches.hpp"
#include "Interface/Interface.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/MeshManager.hpp"
#include "Chest.hpp"
#include "Door.hpp"
#include "Lever.hpp"
#include "Pickup.hpp"
#include "PlayerController.hpp"
#include "Render/GraphRenderer.hpp"
#include "Script/Lua.hpp"
#include <SDL2/SDL.h>
#include <fstream>

#include "SceneGraph/LightNode.hpp"

void Level::initFromScript(const std::string& file)
{
    m_animSys.init();
    m_physSys.init();

    m_lvlContext.level = this;
    m_lvlContext.sceneGraph = &m_sceneGraph;
    m_lvlContext.animSys = &m_animSys;
    m_lvlContext.physSys = &m_physSys;

    m_sceneGraph.init(&m_lvlContext);

    lua::state state;
    state.open_libraries(sol::lib::base);

    state["setLevelMesh"] = [&](const std::string& name)
    {
        setLevelMesh(name, "net.obj");
    };

    state["setPlayerSpawnPoint"] = [&](f32 x, f32 y, f32 z)
    {
        addCreature(Creature::Species::Player)->setPos(vec3(x,y,z));
    };

    state["Species_Skeleton"] = i32(Creature::Species::Skeleton);

    state["addCreature"] = [&](i32 species, f32 x, f32 y, f32 z) -> i32
    {
        addCreature(Creature::Species(species))->setPos(vec3(x,y,z));
        return 123;
    };

    state["addChest"] = [&](f32 x, f32 y, f32 z) -> i32
    {
        addChest("")->setPos(vec3(x,y,z));
        return 123;
    };

    state.script_file("Maps/Level0.lua");

    lua::function init = state["initializeLevel"];
    init();

    auto light = m_sceneGraph.addLightNode();
    auto li = light->as<LightNode>();
    li->setColor(vec3(1,0.75,0.1) * 500);
    li->setShadowCasting(true);
    li->setPosition(vec3(3,3,3));

    m_sceneGraph.getRoot()->attachNode(light);

    ui::g_Interface.setWhoms(m_entities[0]->as<Creature>());

    auto rigud = m_sceneGraph.addSkinnedMeshNode("rigud.dae", "Rigud");
    m_sceneGraph.getRoot()->attachNode(rigud);
}

void Level::setLevelMesh(const std::string& map, const std::string& net)
{
    m_waynet.loadFromFile(net);

    auto mesh = gfx::g_MeshMgr.getMesh(map);

    for (const auto& entry : mesh->entries)
    {
        physx::PxTriangleMeshDesc meshDesc;

        std::vector<vec3> positions;

        for (auto& vert : entry.vertices)
        {
            positions.push_back(vert.pos);
        }

        meshDesc.points.count = entry.vertices.size();
        meshDesc.points.stride = sizeof(vec3);
        meshDesc.points.data = &(positions[0]);

        meshDesc.triangles.count = entry.indices.size() / 3;
        meshDesc.triangles.stride = sizeof(GLuint) * 3;
        meshDesc.triangles.data = &(entry.indices[0]);

        m_physSys.addTriangleMesh(meshDesc);
    }

    m_mapMesh = m_sceneGraph.addMeshNode(map);
    m_sceneGraph.getRoot()->attachNode(m_mapMesh);
}

Creature* Level::addCreature(Creature::Species species)
{
    Entity::Ptr entity(new Creature(m_lastEntityId, &m_lvlContext));

    Creature* creature = entity->as<Creature>();

    creature->init(species);

    if (species == Creature::Species::Player)
        creature->setController(new PlayerController(creature, &m_lvlContext));

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return creature;
}

Pickup* Level::addPickup(const std::string& item)
{
    Entity::Ptr entity(new Pickup(m_lastEntityId, &m_lvlContext));

    Pickup* pickup = entity->as<Pickup>();
    pickup->init();

    m_creationQueue.push_back(std::move(entity));
    m_lastEntityId++;

    return pickup;
}

Chest* Level::addChest(const Code& code)
{
    Entity::Ptr entity(new Chest(m_lastEntityId, &m_lvlContext));

    Chest* chest = entity->as<Chest>();
    chest->init();

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return chest;
}

Door* Level::addDoor(const std::string& code)
{
    Entity::Ptr entity(new Door(m_lastEntityId, &m_lvlContext));

    auto door = entity->as<Door>();
    door->init();

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return door;
}

Lever* Level::addLever(const std::string& code)
{
    Entity::Ptr entity(new Lever(m_lastEntityId, &m_lvlContext));

    auto lever = entity->as<Lever>();
    lever->init();
    lever->setActivationTarget(3);

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return lever;
}

Projectile* Level::addProjectile(Projectile* raw)
{
    Projectile::Ptr proj(raw);

    m_projectiles.push_back(std::move(proj));

    return raw;
}

void Level::sendSystemEvent(const SDL_Event& event)
{

}

void Level::createEntities()
{
    for (auto& ent : m_creationQueue)
        m_entities.push_back(std::move(ent));
    m_creationQueue.clear();
}

void Level::destroyEntities()
{
    for (i32 i = m_entities.size() - 1; i >= 0; i--)
    {
        if (m_entities[i]->isDestroyed())
            m_entities.erase(m_entities.begin() + i);
    }
}

void Level::createProjectiles()
{
    for (auto& ent : m_projectileQueue)
        m_projectiles.push_back(std::move(ent));
    m_projectileQueue.clear();
}

void Level::destroyProjectiles()
{
    for (i32 i = m_projectiles.size() - 1; i >= 0; i--)
    {
        if (m_projectiles[i]->isDestroyed())
            m_projectiles.erase(m_projectiles.begin() + i);
    }
}

void Level::update()
{
    m_physSys.preSimulationUpdate();

    createEntities();
    destroyEntities();

    createProjectiles();
    destroyProjectiles();

    for (auto& ent : m_entities)
        ent->update();

    for (auto& proj : m_projectiles)
        proj->update();

    m_physSys.stepSimulation();

    for (auto& ent : m_entities)
        ent->lateUpdate();

    if (DebugSwitches::drawWaynet)
        m_waynet.debugDraw();

    m_sceneGraph.updateTransforms();

    m_animSys.animate();

    gfx::g_GraphRenderer.render(m_sceneGraph);
}

Interactible* Level::getClosestInteractible(const vec3& pos, const vec3& dir)
{
    Interactible* bestest = nullptr;

    for (auto& ent : m_entities)
    {
        if ((ent->getType() == Entity::Type::Pickup ||
            ent->getType() == Entity::Type::Creature ||
            ent->getType() == Entity::Type::Chest ||
            ent->getType() == Entity::Type::Door ||
            ent->getType() == Entity::Type::Lever) &&
            !ent->isDisabled())
        {
            vec3 flatpos = vec3(pos.x, 0, pos.z);
            vec3 entpos = vec3(ent->getPos().x, 0, ent->getPos().z);

            if (f32 dist = math::distance(flatpos, entpos); dist > 4.f || dist < 0.01f)
                continue;

            vec3 toTheStuff = math::normalize(entpos - flatpos);

            if (math::dot(dir, toTheStuff) < 0.75)
                continue;

            if (bestest)
            {
                f32 distone = math::distance2(pos, bestest->getPos());
                f32 distwo = math::distance2(pos, ent->getPos());

                if (distwo < distone)
                    bestest = ent->as<Interactible>();
            }
            else
                bestest = ent->as<Interactible>();
        }
    }

    return bestest;
}

Creature* Level::getClosestCombatTarget(const vec3& pos, const vec3& dir, bool onlyPlayer)
{
    Creature* bestest = nullptr;

    for (auto& ent : m_entities)
    {
        if (ent->getType() == Entity::Type::Creature)
        {
            if (ent->as<Creature>()->isDead())
                continue;

            if (f32 dist = math::distance(pos, ent->getPos()); dist > 10.f || dist < 0.01f)
                continue;

            vec3 toTheFucker = math::normalize(ent->getPos() - pos);

            if (dir != vec3(0) && math::dot(dir, toTheFucker) < 0.7)
                continue;

            if (ent->as<Creature>()->isPlayer() && onlyPlayer)
                return ent->as<Creature>();

            if (bestest)
            {
                f32 distone = math::distance2(pos, bestest->getPos());
                f32 distwo = math::distance2(pos, ent->getPos());

                if (distwo < distone)
                    bestest = ent->as<Creature>();
            }
            else
                bestest = ent->as<Creature>();
        }
    }

    return bestest;
}

Entity* Level::getEntityById(u32 id) const
{
    for (auto& i : m_entities)
        if (i->getId() == id)
            return i.get();
    return nullptr;
}

Waynet& Level::getWaynet()
{
    return m_waynet;
}
