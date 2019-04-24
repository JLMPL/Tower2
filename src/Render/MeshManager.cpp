#include "MeshManager.hpp"

namespace gfx
{

MeshManager g_MeshMgr;

StaticMesh* MeshManager::getMesh(const String& name)
{
    auto found = m_meshes.find(name);

    if (found != m_meshes.end())
        return (*found).second.get();

    std::unique_ptr<StaticMesh> mesh(new StaticMesh());
    m_staticMeshLoader.loadFromFile(*mesh, String("Meshes/" + name).c_str());
    m_meshes[name] = std::move(mesh);

    return m_meshes[name].get();
}

SkinnedMesh* MeshManager::getSkinnedMesh(const String& name)
{
    auto found = m_skinMeshes.find(name);

    if (found != m_skinMeshes.end())
        return (*found).second.get();

    std::unique_ptr<SkinnedMesh> mesh(new SkinnedMesh());
    m_skinnedMeshLoader.loadFromFile(*mesh, String("Meshes/" + name).c_str());
    m_skinMeshes[name] = std::move(mesh);

    return m_skinMeshes[name].get();
}

}
