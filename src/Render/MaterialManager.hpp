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

    const Material* getMaterial(const String& name);

private:
    void loadMaterialsFile(const Path& path);

private:
    std::map<String, std::unique_ptr<Material>> m_materials;
};

extern MaterialManager g_MatMgr;

}
