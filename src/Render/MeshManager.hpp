#include "Geometry/Mesh.hpp"
#include "Geometry/SkinnedMeshLoader.hpp"
#include "Geometry/StaticMeshLoader.hpp"
#include <map>

namespace gfx
{

class MeshManager
{
public:
    MeshManager() = default;
    ~MeshManager() = default;

    void init();

    Mesh* getMesh(const std::string& name, bool cloth = false);
    Mesh* getSkinnedMesh(const std::string& name, bool cloth = false);

private:
    StaticMeshLoader   m_staticMeshLoader;
    SkinnedMeshLoader  m_skinnedMeshLoader;

    std::map<std::string, std::unique_ptr<Mesh>>  m_cloths;
    std::map<std::string, std::unique_ptr<Mesh>>  m_meshes;
    std::map<std::string, std::unique_ptr<Mesh>> m_skinMeshes;
};

extern MeshManager g_MeshMgr;

}
