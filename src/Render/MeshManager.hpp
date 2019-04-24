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

    StaticMesh* getMesh(const String& name);
    SkinnedMesh* getSkinnedMesh(const String& name);

private:
    StaticMeshLoader   m_staticMeshLoader;
    SkinnedMeshLoader  m_skinnedMeshLoader;

    std::map<String, std::unique_ptr<StaticMesh>>         m_meshes;
    std::map<String, std::unique_ptr<SkinnedMesh>>  m_skinMeshes;
};

extern MeshManager g_MeshMgr;

}
