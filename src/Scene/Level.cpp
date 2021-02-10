#include "Level.hpp"
#include "Render/SceneRenderer.hpp"
#include "Render/Scene/RenderMesh.hpp"
#include "Debug/DebugMenu.hpp"
#include "Resource/MeshCache.hpp"
#include "Components.hpp"
#include <SDL2/SDL.h>

void Level::initFromScript(const std::string& file)
{
    m_physSys.init();

    setLevelMesh("empty.obj", "net.obj");

    m_light = m_renderScene.addRenderLight();
    m_light->setColor(vec3(1,1,1) * 400);
    m_light->setPosition(vec3(0,3,0));

    m_flare = m_renderScene.addRenderFlare("flare.png");
    m_flare->setColor(Color(0.5,0.75,1,1));
    m_flare->setPosition(vec3(0,3,0));

    AnimationCache.load<AnimationLoader>("idle"_hs, "Animations/Idle.dae");
    AnimationCache.load<AnimationLoader>("slash"_hs, "Animations/Slash.dae");
    AnimationCache.load<AnimationLoader>("jump"_hs, "Animations/Jump.dae");
    AnimationCache.load<AnimationLoader>("run_jump"_hs, "Animations/RunningJump.dae");
    AnimationCache.load<AnimationLoader>("kick"_hs, "Animations/Kick.dae");

    debug::g_Menu["Scene"].bind("cameraPos", &m_eye);

    const auto entity = m_registry.create();
    m_registry.emplace<comp::Transform>(entity, mat4(1.f));

    auto leMesh = MeshCache.load<StaticMeshLoader>("le_mesh"_hs, "empty.obj");
    m_registry.emplace<comp::StaticMesh>(entity, m_renderScene.addRenderMesh(leMesh), mat4(1.f));


    const auto archer = m_registry.create();
    m_registry.emplace<comp::Transform>(archer, mat4(1.f));

    m_registry.emplace<comp::SkinnedMesh>(archer);
    auto& skin = m_registry.get<comp::SkinnedMesh>(archer);

    loadSkeletonFromFile(&skin.skeleton, "Meshes/Archer.dae");
    auto mesh = MeshCache.load<SkinnedMeshLoader>("archer"_hs, "Archer.dae", skin.skeleton);
    skin.skinNode = m_renderScene.addRenderSkinnedMesh(mesh, skin.skinData.palette);
    skin.skinNode->setTransform(math::scale(vec3(0.01)));
    skin.anim = AnimationCache.handle("slash"_hs);
}

void Level::setLevelMesh(const std::string& map, const std::string& net)
{
    auto mesh = MeshCache.load<StaticMeshLoader>("map"_hs, map);

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

    // m_mapMesh = m_renderScene.addRenderMesh(mesh);
}

void Level::update()
{
    m_physSys.preSimulationUpdate();

    //update

    m_physSys.stepSimulation();

    auto view = m_registry.view<comp::SkinnedMesh>();

    for (auto& entity : view)
    {
        auto& sc = m_registry.get<comp::SkinnedMesh>(entity);

        sc.along += timer::delta * sc.speed;

        if (sc.along > sc.anim->duration)
        {
            sc.along = 0;
        }

        sc.pose = getSkeletonPose(sc.skeleton, sc.anim, sc.along);
        sc.skinData.computeMatrices(sc.skeleton, sc.pose);
    }



    //post physics update

    // m_pose = getSkeletonPose(m_skeleton, m_animations[m_currentAnim], along);

    // m_skinData.computeMatrices(m_skeleton, m_pose);

    // for (int i = 0; i < m_skinData.transforms.size(); i++)
    // {
    //     m_skinData.transforms[i][3] = vec4(vec3(m_skinData.transforms[i][3]) * 0.01, 1);
    //     // gfx::g_SceneRenderer.addSphere(m_skinData.transforms[i][3], 0.1f, vec3(0,1,0));
    // }

    m_camera.setEye(m_eye);
    // m_camera.setCenter(vec3(m_skinData.transforms[1][3]));
    m_camera.setCenter(vec3(0,1,0));
    m_camera.updateMatrices();
    m_renderScene.setView(m_camera.getView());

    // m_light->setPosition(vec3(skinData.transforms[1][3]) + vec3(1,2,0));

    m_physSys.debugDraw();

    gfx::g_SceneRenderer.addLine(vec3(0,1,0), vec3(1,1,0), vec3(1,0,0));
    gfx::g_SceneRenderer.addLine(vec3(0,1,0), vec3(0,2,0), vec3(0,1,0));
    gfx::g_SceneRenderer.addLine(vec3(0,1,0), vec3(0,1,1), vec3(0,0,1));

    gfx::g_SceneRenderer.render(m_renderScene);
}

void Level::draw()
{
}
