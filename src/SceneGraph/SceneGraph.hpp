#pragma once
#include "SceneNode.hpp"

namespace gfx
{
    class StaticMesh;
    class Mesh;
    class GraphRenderer;
};

class MeshNode;
class LightNode;
class CameraNode;
class SkinnedMeshNode;
class FlareNode;
class ClothNode;

struct LevelContext;

namespace phys
{
    class Cloth;
}

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
    ClothNode* addClothNode(phys::Cloth* cloth);

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
