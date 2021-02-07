#include "Level.hpp"
#include "Render/SceneRenderer.hpp"
#include "Render/Scene/RenderMesh.hpp"
#include "Debug/DebugMenu.hpp"
#include "Resource/MeshCache.hpp"
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

    m_animations.push_back(AnimationCache.load<AnimationLoader>("idle"_hs, "Animations/Idle.dae"));
    m_animations.push_back(AnimationCache.load<AnimationLoader>("slash"_hs, "Animations/Slash.dae"));
    m_animations.push_back(AnimationCache.load<AnimationLoader>("jump"_hs, "Animations/Jump.dae"));
    m_animations.push_back(AnimationCache.load<AnimationLoader>("run_jump"_hs, "Animations/RunningJump.dae"));
    m_animations.push_back(AnimationCache.load<AnimationLoader>("kick"_hs, "Animations/Kick.dae"));

    debug::g_Menu["Scene"].bind("animIndex", &m_nextAnim);
    debug::g_Menu["Scene"].bind("cameraPos", &m_eye);
    debug::g_Menu["Scene"].bind("speed", &m_speed);

    auto mesh = MeshCache.load<SkinnedMeshLoader>("archer"_hs, "Archer.dae");
    m_skeleton = &mesh->skeleton;

    m_matrixPalette.resize(m_skeleton->joints.size());
    m_jointTransforms.resize(m_skeleton->joints.size());

    m_rawskin = m_renderScene.addRenderSkinnedMesh(mesh, m_matrixPalette.data());
    m_rawskin->setTransform(math::scale(vec3(0.01)));
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

    m_mapMesh = m_renderScene.addRenderMesh(mesh);
}

void Level::update()
{
    m_physSys.preSimulationUpdate();

    //update

    m_physSys.stepSimulation();

    along += timer::delta * m_speed;
    if (along >= m_animations[m_currentAnim]->duration)
    {
        along = 0;

        if (m_nextAnim >= m_animations.size())
            m_nextAnim = m_animations.size()-1;

        m_currentAnim = m_nextAnim;
    }

    //post physics update

    m_pose = getSkeletonPose(*m_skeleton, m_animations[m_currentAnim], along);

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
