#include "Level.hpp"
#include "Render/SceneRenderer.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/MeshManager.hpp"
#include "Render/Scene/RenderMesh.hpp"
#include "Debug/DebugMenu.hpp"
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

    loadAnimationFromFile(&m_animations[0], "Animations/Clips/Walk.dae");
    loadAnimationFromFile(&m_animations[1], "Meshes/StandingDeath.dae");
    loadAnimationFromFile(&m_animations[2], "Animations/Clips/Jump.dae");
    loadAnimationFromFile(&m_animations[3], "Animations/Clips/RunningJump.dae");

    debug::g_Menu["Scene"].bind("animIndex", &m_nextAnim);
    debug::g_Menu["Scene"].bind("cameraPos", &m_eye);

    m_skeleton = &gfx::g_MeshMgr.getSkinnedMesh("Archer.dae")->skeleton;

    m_matrixPalette.resize(m_skeleton->joints.size());
    m_jointTransforms.resize(m_skeleton->joints.size());

    m_rawskin = m_renderScene.addRenderSkinnedMesh("Archer.dae", m_matrixPalette.data());
    m_rawskin->setTransform(math::scale(vec3(0.01)));
}

void Level::setLevelMesh(const std::string& map, const std::string& net)
{
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

void Level::update()
{
    m_physSys.preSimulationUpdate();

    //update

    m_physSys.stepSimulation();

    along += timer::delta;
    if (along >= m_animations[m_currentAnim].duration)
    {
        along = 0;
        m_currentAnim = m_nextAnim;
    }

    //post physics update

    m_pose = getSkeletonPose(m_skeleton, &m_animations[m_currentAnim], along);

    computeSkinMatrices(*m_skeleton, m_pose, m_matrixPalette, m_jointTransforms);

    for (int i = 0; i < m_jointTransforms.size(); i++)
    {
        m_jointTransforms[i][3] = vec4(vec3(m_jointTransforms[i][3]) * 0.01, 1);
    }

    m_camera.setEye(m_eye);
    m_camera.setCenter(vec3(m_jointTransforms[1][3]));
    m_camera.updateMatrices();
    m_renderScene.setView(m_camera.getView());

    m_light->setPosition(vec3(m_jointTransforms[1][3]) + vec3(1,2,0));

    m_physSys.debugDraw();

    gfx::g_SceneRenderer.addLine(vec3(0,1,0), vec3(1,1,0), vec3(1,0,0));
    gfx::g_SceneRenderer.addLine(vec3(0,1,0), vec3(0,2,0), vec3(0,1,0));
    gfx::g_SceneRenderer.addLine(vec3(0,1,0), vec3(0,1,1), vec3(0,0,1));

    gfx::g_SceneRenderer.render(m_renderScene);
}

void Level::draw()
{
}
