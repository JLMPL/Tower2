#pragma once
#include "Render/Shader.hpp"
#include <unordered_map>

namespace gfx
{

class ShaderManager
{
public:
    ShaderManager() = default;

    const Shader* getShader(const std::string& name, Shader::Type type = Shader::Type::Invalid);

private:
    const Shader* getMonolithicShader(const std::string& name);
    const Shader* getModularShader(Shader::Type type, const std::string& name);

private:
    std::unordered_map<std::string, Shader::Ptr> m_monolithicShaders;
    std::unordered_map<std::string, Shader::Ptr> m_modularShaders[Shader::Type::Count];
};

extern ShaderManager g_ShaderMgr;

}
