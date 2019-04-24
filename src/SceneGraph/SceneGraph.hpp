#pragma once
#include "SceneNode.hpp"

/*

LocatorNode
MeshNode
LightNode
SkinMeshNode
CameraNode

*/

//Purely for rendering

namespace gfx
{
    class StaticMesh;
    class SkinnedMesh;
    class GraphRenderer;
};

class MeshNode;
class LightNode;
class CameraNode;

class SceneGraph
{
public:
    SceneGraph() = default;
    ~SceneGraph() = default;

    SceneNode* addEmptyNode();
    SceneNode* addMeshNode(const std::string& mesh);
    SceneNode* addSkinnedMeshNode(const std::string& mesh, const std::string& bundle);
    SceneNode* addLightNode();
    SceneNode* addCameraNode();

    void updateTransforms();

    SceneNode* getRoot();

    std::vector<SceneNode::Ptr>& getNodes();

private:
    SceneNode m_root;

    std::vector<SceneNode::Ptr> m_nodes;

    friend class gfx::GraphRenderer;
};
