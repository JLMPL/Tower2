#pragma once
#include "Render/Geometry/SkinnedMesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

namespace gfx
{

class SkinnedMesh;

class SkinnedMeshLoader
{
public:
    SkinnedMeshLoader() = default;
    ~SkinnedMeshLoader() = default;

    void loadFromFile(SkinnedMesh& mesh, const std::string& path, bool cloth = false);

private:
    i8 addJointsToSkeleton(SkinnedMesh& mesh, const aiNode& node);
    void addMeshesAndJoints(SkinnedMesh& mesh, const aiScene& scene, bool cloth);
    void doTheShitWithWeights(SkinnedMesh& mesh, const aiMesh& inMesh, SkinnedMesh::Entry& entry, bool cloth);
    void genBufferObjects(SkinnedMesh::Entry& entry);

private:
    std::vector<i32> m_redirect;
};

}
