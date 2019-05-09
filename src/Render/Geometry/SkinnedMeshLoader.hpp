#pragma once
#include "Render/Geometry/SkinnedMesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

namespace gfx
{

class Resources;
class SkinnedMesh;

class SkinnedMeshLoader
{
public:
    SkinnedMeshLoader() = default;
    ~SkinnedMeshLoader() = default;

    void loadFromFile(SkinnedMesh& mesh, const std::string& path);

private:
    i8 addNodesToSkeleton(SkinnedMesh& mesh, const aiNode& node);
    void addMeshesAndJoints(SkinnedMesh& mesh, const aiScene& scene);
    void doTheShitWithWeights(SkinnedMesh& mesh, const aiMesh& inMesh, SkinnedMesh::Entry& entry);
    void genBufferObjects(SkinnedMesh::Entry& entry);

private:
    Resources* m_res = nullptr;
};

}
