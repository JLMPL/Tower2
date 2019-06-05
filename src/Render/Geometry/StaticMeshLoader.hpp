#pragma once
#include "Core/Math3D.hpp"
#include "Render/Geometry/StaticMesh.hpp"
#include <vector>
#include <string>

struct aiMesh;
struct aiScene;

namespace gfx
{

class StaticMeshLoader
{
public:
    StaticMeshLoader() = default;
    ~StaticMeshLoader() = default;

    void loadFromFile(StaticMesh& mesh, const std::string& path, bool cloth = false);

private:
    void loadMesh(StaticMesh& mesh, const aiScene& scene, const aiMesh& inMesh);
    void loadClothMesh(StaticMesh& mesh, const aiScene& scene, const aiMesh& inMesh);
    void setupBuffers(StaticMesh::Entry& ent);

private:
};

}
