#pragma once
#include "Core/Math3D.hpp"
#include "Render/Geometry/Mesh.hpp"
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

    void loadFromFile(Mesh& mesh, const std::string& path, bool cloth = false);

private:
    void loadMesh(Mesh& mesh, const aiScene& scene, const aiMesh& inMesh, bool isDae);
    void loadClothMesh(Mesh& mesh, const aiScene& scene, const aiMesh& inMesh, bool isDae);
    void setupBuffers(Mesh::Entry& ent);

private:
};

}
