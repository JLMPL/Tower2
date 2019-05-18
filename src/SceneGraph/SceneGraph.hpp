#pragma once
#include "SceneNode.hpp"

namespace gfx
{
    class StaticMesh;
    class SkinnedMesh;
    class GraphRenderer;
};

class MeshNode;
class LightNode;
class CameraNode;
class SkinnedMeshNode;
class FlareNode;

struct LevelContext;

class SceneGraph
{
public:
    SceneGraph() = default;
    ~SceneGraph() = default;

    void init(LevelContext* context);

    SceneNode* addEmptyNode();
    MeshNode* addMeshNode(const std::string& mesh);
    SkinnedMeshNode* addSkinnedMeshNode(const std::string& mesh, const std::string& bundle);
    LightNode* addLightNode();
    CameraNode* addCameraNode();
    FlareNode* addFlareNode(const std::string& tex);

    void updateTransforms();

    SceneNode* getRoot();

    std::vector<SceneNode::Ptr>& getNodes();
    const CameraNode& getCamera() const;

private:
    LevelContext* m_context = nullptr;
    SceneNode m_root;

    CameraNode* m_camera = nullptr;

    std::vector<SceneNode::Ptr> m_nodes;

    friend class gfx::GraphRenderer;
};
