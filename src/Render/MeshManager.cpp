#include "MeshManager.hpp"

namespace gfx
{

MeshManager g_MeshMgr;

Mesh* MeshManager::getMesh(const std::string& name, bool cloth)
{
    auto found = m_meshes.find(name);

    if (found != m_meshes.end())
        return (*found).second.get();

    std::unique_ptr<Mesh> mesh(new Mesh());
    loadStaticMeshFromFile(*mesh, std::string("Meshes/" + name).c_str(), cloth);
    m_meshes[name] = std::move(mesh);

    return m_meshes[name].get();
}

Mesh* MeshManager::getSkinnedMesh(const std::string& name, bool cloth)
{
    auto found = m_skinMeshes.find(name);

    if (found != m_skinMeshes.end())
        return (*found).second.get();

    std::unique_ptr<Mesh> mesh(new Mesh());
    loadSkinnedMeshFromFile(*mesh, std::string("Meshes/" + name).c_str(), cloth);
    m_skinMeshes[name] = std::move(mesh);

    return m_skinMeshes[name].get();
}

}
