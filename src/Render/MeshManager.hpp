#include "Geometry/SkinnedMesh.hpp"
#include "Geometry/SkinnedMeshLoader.hpp"
#include "Geometry/StaticMesh.hpp"
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

    StaticMesh* getMesh(const std::string& name, bool cloth = false);
    SkinnedMesh* getSkinnedMesh(const std::string& name);

private:
    StaticMeshLoader   m_staticMeshLoader;
    SkinnedMeshLoader  m_skinnedMeshLoader;

    std::map<std::string, std::unique_ptr<StaticMesh>>  m_cloths;
    std::map<std::string, std::unique_ptr<StaticMesh>>  m_meshes;
    std::map<std::string, std::unique_ptr<SkinnedMesh>> m_skinMeshes;
};

extern MeshManager g_MeshMgr;

}
