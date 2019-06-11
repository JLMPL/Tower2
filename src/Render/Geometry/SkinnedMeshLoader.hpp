#pragma once
#include "Render/Geometry/Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

namespace gfx
{

class Mesh;

class SkinnedMeshLoader
{
public:
    SkinnedMeshLoader() = default;
    ~SkinnedMeshLoader() = default;

    void loadFromFile(Mesh& mesh, const std::string& path, bool cloth = false);

private:
    i8 addJointsToSkeleton(Mesh& mesh, const aiNode& node);
    void addMeshesAndJoints(Mesh& mesh, const aiScene& scene, bool cloth);
    void doTheShitWithWeights(Mesh& mesh, const aiMesh& inMesh, Mesh::Entry& entry, bool cloth);
    void genBufferObjects(Mesh::Entry& entry);

private:
    std::vector<i32> m_redirect;
};

}
