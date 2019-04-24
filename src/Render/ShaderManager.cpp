#include "ShaderManager.hpp"

namespace gfx
{

ShaderManager g_ShaderMgr;

const std::string vert[]
{
    "Shaders/Static.vert",
    "Shaders/StaticWavy.vert",
    "Shaders/Skinned.vert"
};

const Shader* ShaderManager::getMonolithicShader(const std::string& name)
{
    auto found = m_monolithicShaders.find(name);

    if (found == m_monolithicShaders.end())
    {
        Shader::Ptr shader;
        shader->loadFromFile("Shaders/" + name);

        m_monolithicShaders[name] = std::move(shader);
    }

    return m_monolithicShaders[name].get();
}

const Shader* ShaderManager::getModularShader(Shader::Type type, const std::string& name)
{
    auto found = m_modularShaders[type].find(name);

    if (found == m_modularShaders[type].end())
    {
        Shader::Ptr shader(new Shader());
        shader->loadFromFile(vert[type], "Shaders/" + name);

        m_modularShaders[type][name] = std::move(shader);
    }

    return m_modularShaders[type][name].get();
}

const Shader* ShaderManager::getShader(const std::string& name, Shader::Type type)
{
    if (type == Shader::Type::Invalid)
        return getMonolithicShader(name);

    return getModularShader(type, name);
}

}
