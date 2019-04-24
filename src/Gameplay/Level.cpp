#include "Level.hpp"
#include "Debug/Switches.hpp"
#include "Interface/Interface.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/MeshManager.hpp"
#include "Chest.hpp"
#include "Creature.hpp"
#include "Door.hpp"
#include "Lever.hpp"
#include "Pickup.hpp"
#include "PlayerController.hpp"
#include "Render/GraphRenderer.hpp"
#include <SDL2/SDL.h>
#include <fstream>

#include "SceneGraph/LightNode.hpp"

void Level::loadFromFile(const Path& path)
{
    std::ifstream file(path);
    json data;
    file >> data;

    m_name = data["name"].get<std::string>();

    std::string mapMesh = data["mapMesh"].get<std::string>();
    std::string waynetMesh = data["waynetMesh"].get<std::string>();

    loadMap(mapMesh, waynetMesh);
    loadEntities(data["entities"]);

    m_cameraController.init(m_entities[0]->as<Creature>());

    auto light = m_sceneGraph.addLightNode();

    auto li = static_cast<LightNode*>(light);
    li->setColor(vec3(1,0.75,0.01) * 500);
    li->setShadowCasting(true);
    li->setPosition(vec3(0,3,0));

    // auto empty = m_sceneGraph.addEmptyNode();

    // empty->attachNode(light);

    // auto camra = m_sceneGraph.addCameraNode();

    auto ass = m_sceneGraph.addMeshNode("pantses.obj");
    // ass->hide();

    // m_sceneGraph.getRoot()->attachNode(empty);
    m_sceneGraph.getRoot()->attachNode(light);
    m_sceneGraph.getRoot()->attachNode(ass);
}

void Level::loadMap(const std::string& map, const std::string& net)
{
    m_waynet.loadFromFile(net);

    auto mesh = gfx::g_MeshMgr.getMesh(map);
    auto mat = gfx::g_MatMgr.getMaterial("env_wall");

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

        phys::g_PhysSys.addTriangleMesh(meshDesc);
    }

    m_mapMesh = m_sceneGraph.addMeshNode(map);
    m_sceneGraph.getRoot()->attachNode(m_mapMesh);
}

Creature* Level::addCreature(const std::string& desc, bool isPlayer)
{
    Entity::Ptr entity(new Creature());

    Creature* creature = entity->as<Creature>();

    creature->init(m_lastEntityId, this, &m_sceneGraph, isPlayer ? Creature::Species::Player : Creature::Species::Skeleton);

    if (isPlayer)
        creature->setController(new PlayerController(creature));

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return creature;
}

Pickup* Level::addPickup(const std::string& item)
{
    Entity::Ptr entity(new Pickup());

    Pickup* pickup = entity->as<Pickup>();
    pickup->init(m_lastEntityId, "pik_damn_herb");

    m_creationQueue.push_back(std::move(entity));
    m_lastEntityId++;

    return pickup;
}

Chest* Level::addChest(const Code& code)
{
    Entity::Ptr entity(new Chest());

    Chest* chest = entity->as<Chest>();
    chest->init(m_lastEntityId, code);

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return chest;
}

Door* Level::addDoor(const std::string& code)
{
    Entity::Ptr entity(new Door());

    auto door = entity->as<Door>();
    door->init(m_lastEntityId, code);

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return door;
}

Lever* Level::addLever(const std::string& code)
{
    Entity::Ptr entity(new Lever());

    auto lever = entity->as<Lever>();
    lever->init(m_lastEntityId, code);
    lever->setActivationTarget(3);

    m_entities.push_back(std::move(entity));
    m_lastEntityId++;

    return lever;
}

void Level::loadCreature(const json& json)
{
    u32 id = json["id"].get<u32>();
    std::string desc = json["creatureDesc"].get<std::string>();

    auto creature = addCreature(desc, !id);

    auto& position = json["position"];
    creature->setPos(vec3(
        position[0].get<f32>(),
        position[1].get<f32>(),
        position[2].get<f32>()
    ));

    auto& items = json["items"];

    for (u32 i = 0; i < items.size(); i++)
    {
        std::string code = items[i]["code"].get<std::string>();
        u32 count = items[i]["count"].get<u32>();

        creature->getEquipment()->give(code, count);
    }
}

void Level::loadChest(const json& json)
{
    std::string code = json["code"].get<std::string>();

    auto chest = addChest(code);

    auto& position = json["position"];
    chest->setPos(vec3(
        position[0].get<f32>(),
        position[1].get<f32>(),
        position[2].get<f32>()
    ));

    auto& items = json["items"];

    for (u32 i = 0; i < items.size(); i++)
    {
        std::string code = items[i]["code"].get<std::string>();
        u32 count = items[i]["count"].get<u32>();

        chest->getEquipment().give(code, count);
    }
}

void Level::loadDoor(const json& json)
{
    std::string code = json["code"].get<std::string>();
    std::string keyItem = json["keyItem"].get<std::string>();

    auto door = addDoor(code);
    door->setKeyItem(keyItem);

    auto& position = json["position"];
    door->setPos(vec3(
        position[0].get<f32>(),
        position[1].get<f32>(),
        position[2].get<f32>()
    ));
}

void Level::loadLever(const json& json)
{
    std::string code = json["code"].get<std::string>();

    auto lever = addLever(code);

    auto& position = json["position"];
    lever->setPos(vec3(
        position[0].get<f32>(),
        position[1].get<f32>(),
        position[2].get<f32>()
    ));
}

void Level::loadEntities(const json& json)
{
    for (u32 i = 0; i < json.size(); i++)
    {
        u32 id = json[i]["id"].get<u32>();
        std::string type = json[i]["type"].get<std::string>();

        if (type == "creature")
        {
            loadCreature(json[i]);
        }
        else if (type == "chest")
        {
            loadChest(json[i]);
        }
        else if (type == "door")
        {
            loadDoor(json[i]);
        }
        else if (type == "lever")
        {
            loadLever(json[i]);
        }
    }

    auto player = m_entities[0]->as<Creature>();

    ui::g_Interface.setWhoms(player, player->getEquipment());
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
    createEntities();
    destroyEntities();

    createProjectiles();
    destroyProjectiles();

    for (auto& ent : m_entities)
        ent->update();

    for (auto& proj : m_projectiles)
        proj->update();
}

void Level::lateUpdate()
{
    for (auto& ent : m_entities)
        ent->lateUpdate();

    m_cameraController.update();

    if (DebugSwitches::drawWaynet)
        m_waynet.debugDraw();

    m_sceneGraph.updateTransforms();

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
