#include "SceneGraph.hpp"
#include "Render/MeshManager.hpp"
#include "Render/TextureManager.hpp"
#include "Gameplay/Level.hpp"
#include "MeshNode.hpp"
#include "SkinnedMeshNode.hpp"
#include "LightNode.hpp"
#include "CameraNode.hpp"
#include "FlareNode.hpp"

void SceneGraph::init(LevelContext* context)
{
    m_context = context;
}

SceneNode* SceneGraph::addEmptyNode()
{
    m_nodes.emplace_back(new SceneNode());
    return m_nodes.back().get();
}

SceneNode* SceneGraph::addMeshNode(const std::string& mesh)
{
    m_nodes.emplace_back(new MeshNode(gfx::g_MeshMgr.getMesh(mesh)));
    return m_nodes.back().get();
}

SceneNode* SceneGraph::addSkinnedMeshNode(const std::string& mesh, const std::string& bundle)
{
    m_nodes.emplace_back(new SkinnedMeshNode(m_context->animSys, gfx::g_MeshMgr.getSkinnedMesh(mesh), bundle));
    return m_nodes.back().get();
}

SceneNode* SceneGraph::addLightNode()
{
    m_nodes.emplace_back(new LightNode(vec3(50)));
    return m_nodes.back().get();
}

SceneNode* SceneGraph::addCameraNode()
{
    m_nodes.emplace_back(new CameraNode());
    return m_nodes.back().get();
}

SceneNode* SceneGraph::addFlareNode(const std::string& tex)
{
    m_nodes.emplace_back(new FlareNode(gfx::g_TexMgr.getTexture(tex)));
    return m_nodes.back().get();
}

SceneNode* SceneGraph::getRoot()
{
    return &m_root;
}

std::vector<SceneNode::Ptr>& SceneGraph::getNodes()
{
    return m_nodes;
}

void SceneGraph::updateTransforms()
{
    m_root.updateTransforms(mat4(1.f));
}
