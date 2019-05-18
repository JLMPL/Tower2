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

MeshNode* SceneGraph::addMeshNode(const std::string& mesh)
{
    m_nodes.emplace_back(new MeshNode(gfx::g_MeshMgr.getMesh(mesh)));
    return m_nodes.back()->as<MeshNode>();
}

SkinnedMeshNode* SceneGraph::addSkinnedMeshNode(const std::string& mesh, const std::string& bundle)
{
    m_nodes.emplace_back(new SkinnedMeshNode(m_context->animSys, gfx::g_MeshMgr.getSkinnedMesh(mesh), bundle));
    return m_nodes.back()->as<SkinnedMeshNode>();
}

LightNode* SceneGraph::addLightNode()
{
    m_nodes.emplace_back(new LightNode(vec3(50)));
    return m_nodes.back()->as<LightNode>();
}

CameraNode* SceneGraph::addCameraNode()
{
    m_nodes.emplace_back(new CameraNode());
    m_camera = m_nodes.back()->as<CameraNode>();
    return m_nodes.back()->as<CameraNode>();
}

FlareNode* SceneGraph::addFlareNode(const std::string& tex)
{
    m_nodes.emplace_back(new FlareNode(gfx::g_TexMgr.getTexture(tex)));
    return m_nodes.back()->as<FlareNode>();
}

SceneNode* SceneGraph::getRoot()
{
    return &m_root;
}

std::vector<SceneNode::Ptr>& SceneGraph::getNodes()
{
    return m_nodes;
}

const CameraNode& SceneGraph::getCamera() const
{
    return *m_camera;
}

void SceneGraph::updateTransforms()
{
    m_root.updateTransforms(mat4(1.f));
}
