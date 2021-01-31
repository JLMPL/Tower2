#include "Level.hpp"
#include "Controllers/PlayerController.hpp"
#include "Entities/LightEffect.hpp"
#include "Render/SceneRenderer.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/MeshManager.hpp"
#include "Render/Scene/RenderMesh.hpp"
#include "Core/Random.hpp"
#include <SDL2/SDL.h>

void Level::initFromScript(const std::string& file)
{
    m_physSys.init();

    m_lvlContext.level       = this;
    m_lvlContext.renderScene = &m_renderScene;
    // m_lvlContext.eventSys    = &m_eventSys;
    // m_lvlContext.animSys     = &m_animSys;
    m_lvlContext.physSys     = &m_physSys;
    m_lvlContext.particleSys = &m_particleSystem;
    m_lvlContext.camera      = &m_camera;

    m_hud.init(m_renderScene);

    setLevelMesh("empty.obj", "net.obj");
    addCreature(Creature::Species::Player, vec3(0,0,0));

    m_cameraCtrl.init(&m_lvlContext);

    addLightEffect(vec3(0,3,0));

    // m_particleGroup = m_particleSystem.addParticleGroup(1024, vec3(0));
    // m_particleAffector = m_particleGroup->addParticleAffector();
    // m_particleAffector->setPosition(vec3(3,2,0));
    // m_particleAffector->setStrength(25);

    // m_renderParticles = m_renderScene.addRenderParticles(m_particleGroup);
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
    // if (event.getType() == GameEvent::Type::SpawnPickup)
    {
        // addPickup(event.pickup.itemID,
            // vec3(event.pickup.x, event.pickup.y, event.pickup.z));
    }
}

void Level::update()
{
    createEntities();
    destroyEntities();

    // for (auto i = 0; i < 8; i++)
    // {
    //     Particle p;
    //     p.pos = vec3(0,2,0);
    //     p.vel = vec3(core::rand::inRange(-2.f,2.f), core::rand::inRange(-2.f,2.f), core::rand::inRange(-2.f,2.f));
    //     p.weight = 0.1;
    //     p.lifetime = 2;
    //     m_particleGroup->spawnParticle(p);
    // }

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

    // m_particleAffector->setPosition(m_entities[0]->getPos() + vec3(0,1,0));

    // m_particleSystem.update();

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
