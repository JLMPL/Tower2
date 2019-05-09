#pragma once
#include "Render/Material.hpp"
#include <map>

namespace gfx
{

class Resources;

class MaterialManager
{
public:
    MaterialManager() = default;

    void loadMaterials();

    const Material* getMaterial(const std::string& name);

private:
    void loadMaterialsFile(const std::string& path);

private:
    std::map<std::string, std::unique_ptr<Material>> m_materials;
};

extern MaterialManager g_MatMgr;

}
