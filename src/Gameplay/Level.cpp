#include "Level.hpp"
#include "Controllers/PlayerController.hpp"
#include "Controllers/SkeletonController.hpp"
#include "Entities/Chest.hpp"
#include "Entities/Door.hpp"
#include "Entities/Lever.hpp"
#include "Entities/Pickup.hpp"
#include "Entities/LightEffect.hpp"
#include "Render/SceneRenderer.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/MeshManager.hpp"
#include "Script/Lua.hpp"
#include "Render/Scene/RenderMesh.hpp"
#include "Core/Random.hpp"
#include <SDL2/SDL.h>

void Level::initFromScript(const std::string& file)
{
    m_physSys.init();

    m_lvlContext.level       = this;
    m_lvlContext.renderScene = &m_renderScene;
    m_lvlContext.eventSys    = &m_eventSys;
    // m_lvlContext.animSys     = &m_animSys;
    m_lvlContext.physSys     = &m_physSys;
    m_lvlContext.particleSys = &m_particleSystem;
    m_lvlContext.camera      = &m_camera;

    m_hud.init(m_renderScene);

    lua::state state;
    state.open_libraries(sol::lib::base);
    uploadFunctions(state);
    state.script_file("Maps/Level0.lua");

    lua::function init = state["initializeLevel"];
    init();

    m_cameraCtrl.init(&m_lvlContext);

    addLightEffect(vec3(0,3,0));

    m_particleGroup = m_particleSystem.addParticleGroup(1024, vec3(0));
    m_particleAffector = m_particleGroup->addParticleAffector();
    m_particleAffector->setPosition(vec3(3,2,0));
    m_particleAffector->setStrength(25);

    m_renderParticles = m_renderScene.addRenderParticles(m_particleGroup);
}

void Level::uploadFunctions(lua::state& state)
{
    state["setLevelMesh"] = [&](const std::string& name)
    {
        setLevelMesh(name, "net.obj");
    };

    state["setPlayerSpawnPoint"] = [&](f32 x, f32 y, f32 z)
    {
        addCreature(Creature::Species::Player, vec3(x,y,z));
    };

    state["Species_Skeleton"] = i32(Creature::Species::Skeleton);

    state["addCreature"] = [&](i32 species, f32 x, f32 y, f32 z) -> i32
    {
        return addCreature(Creature::Species(species), vec3(x,y,z));
    };

    state["addChest"] = [&](f32 x, f32 y, f32 z) -> i32
    {
        return addChest(vec3(x,y,z));
    };

    state["addDoor"] = [&](f32 x, f32 y, f32 z) -> i32
    {
        return addDoor(vec3(x,y,z));
    };
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

    m_mapMesh = m_renderScene.addRenderMesh(map);
}

u32 Level::addCreature(Creature::Species species, const vec3& pos)
{
    Entity::Ptr entity(new Creature(m_lastEntityId, &m_lvlContext, species));

    auto creature = entity->as<Creature>();
    creature->setPos(pos);

    if (species == Creature::Species::Player)
        m_controllers.emplace_back(new PlayerController(creature, &m_lvlContext));
    else if (species == Creature::Species::Skeleton)
        m_controllers.emplace_back(new SkeletonController(creature, &m_lvlContext));

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return m_lastEntityId-1;
}

u32 Level::addPickup(u32 item, const vec3& pos)
{
    Entity::Ptr entity(new Pickup(m_lastEntityId, &m_lvlContext));
    // entity->setPos(pos);

    Pickup* pickup = entity->as<Pickup>();
    pickup->getRigidBody().setGlobalTransform(math::translate(pos));

    m_creationQueue.push_back(std::move(entity));
    m_lastEntityId++;

    return m_lastEntityId-1;
}

u32 Level::addChest(const vec3& pos)
{
    Entity::Ptr entity(new Chest(m_lastEntityId, &m_lvlContext));
    entity->setPos(pos);

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return m_lastEntityId-1;
}

u32 Level::addDoor(const vec3& pos)
{
    Entity::Ptr entity(new Door(m_lastEntityId, &m_lvlContext));
    entity->setPos(pos);

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return m_lastEntityId-1;
}

u32 Level::addLever(u32 target, const vec3& pos)
{
    Entity::Ptr entity(new Lever(m_lastEntityId, &m_lvlContext));

    auto lever = entity->as<Lever>();
    lever->setActivationTarget(3);
    lever->setPos(pos);

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return m_lastEntityId-1;
}

void Level::addLightEffect(vec3 pos)
{
    Entity::Ptr entity(new LightEffect(m_lastEntityId, &m_lvlContext, 0));
    entity->setPos(pos);

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;
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

void Level::onEvent(const GameEvent& event)
{
    if (event.getType() == GameEvent::Type::SpawnPickup)
    {
        addPickup(event.pickup.itemID,
            vec3(event.pickup.x, event.pickup.y, event.pickup.z));
    }
}

void Level::update()
{
    for (auto& event : m_eventSys.getEvents())
    {
        onEvent(event);

        for (auto& ent : m_entities)
            ent->onEvent(event);

        for (auto& ctrl : m_controllers)
            ctrl->onEvent(event);

        m_hud.onEvent(event);
    }
    m_eventSys.clear();

    createEntities();
    destroyEntities();

    for (auto i = 0; i < 8; i++)
    {
        Particle p;
        p.pos = vec3(0,2,0);
        p.vel = vec3(core::rand::inRange(-2.f,2.f), core::rand::inRange(-2.f,2.f), core::rand::inRange(-2.f,2.f));
        p.weight = 0.1;
        p.lifetime = 2;
        m_particleGroup->spawnParticle(p);
    }

    m_cameraCtrl.updateCameraRotation();

    for (auto& ctrl : m_controllers)
        ctrl->update();

    m_physSys.preSimulationUpdate();

    for (auto& ent : m_entities)
        ent->update();

    for (auto& ctrl : m_controllers)
        ctrl->preSimulationUpdate();

    m_physSys.stepSimulation();

    for (auto& ent : m_entities)
        ent->lateUpdate();

    m_cameraCtrl.updateCameraPosition();
    // m_waynet.debugDraw();

    anim::updateAnimationSystem();

    m_particleAffector->setPosition(m_entities[0]->getPos() + vec3(0,1,0));

    m_particleSystem.update();

    m_physSys.debugDraw();

    gfx::g_SceneRenderer.addLine(vec3(0,1,0), vec3(1,1,0), vec3(1,0,0));
    gfx::g_SceneRenderer.addLine(vec3(0,1,0), vec3(0,2,0), vec3(0,1,0));
    gfx::g_SceneRenderer.addLine(vec3(0,1,0), vec3(0,1,1), vec3(0,0,1));

    gfx::g_SceneRenderer.render(m_renderScene);

    m_hud.update();
}

void Level::draw()
{
    m_hud.draw();
}

Interactible* Level::getClosestInteractible(const vec3& pos, const vec3& dir)
{
    Interactible* bestest = nullptr;

    for (auto& ent : m_entities)
    {
        if (ent->getType() == Entity::Type::Pickup ||
            ent->getType() == Entity::Type::Chest ||
            ent->getType() == Entity::Type::Lever)
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

            if (ent->as<Creature>()->getSpecies() == Creature::Species::Player && onlyPlayer)
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

Entity* Level::getEntityByID(u32 id)
{
    for (auto& ent : m_entities)
    {
        if (ent->getID() == id)
            return ent.get();
    }

    return nullptr;
}

Waynet& Level::getWaynet()
{
    return m_waynet;
}
