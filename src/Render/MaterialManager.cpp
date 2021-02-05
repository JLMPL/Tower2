#include "MaterialManager.hpp"
#include "Debug/Log.hpp"
#include <fstream>

namespace gfx
{

MaterialManager g_MatMgr;

void MaterialManager::loadMaterials()
{
    loadMaterialsFile("Materials/materials.json");
}

void MaterialManager::loadMaterialsFile(const std::string& path)
{
    std::ifstream file(path.c_str());
    std::json materials;
    file >> materials;

    for (u32 i = 0; i < materials["materials"].size(); i++)
    {
        auto& jmat = materials["materials"][i];

        std::string name = jmat["name"].get<std::string>();

        std::unique_ptr<Material> mat(new Material());
        mat->load(jmat);

        m_materials[name] = std::move(mat);
    }
}

const Material* MaterialManager::getMaterial(const std::string& name)
{
    auto found = m_materials.find(name);

    if (found != m_materials.end())
        return (*found).second.get();

    Log::warning("No such material %s\n", name.c_str());
    return m_materials["env_empty"].get();
    // return nullptr;
}

}
