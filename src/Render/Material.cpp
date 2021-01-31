#include "Material.hpp"
#include "Debug/Log.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"

namespace gfx
{

void Material::load(std::json& node)
{
    std::string name = node["name"];

    if (!node["albedo"].is_null())
    {
        m_textures[0] = g_TexMgr.getTexture(node["albedo"].get<std::string>());
    }
    else
        Log::error("Material %s does not contain albedo map!\n", name.c_str());

    if (!node["normal"].is_null())
    {
        m_textures[1] = g_TexMgr.getTexture(node["normal"].get<std::string>());
    }
    else
        Log::error("Material %s does not contain normal map!\n", name.c_str());

    if (!node["specular"].is_null())
    {
        m_textures[2] = g_TexMgr.getTexture(node["specular"].get<std::string>());
    }
    else
        Log::error("Material %s does not contain specular map!\n", name.c_str());

    m_shader[Shader::Type::Static] =
        g_ShaderMgr.getShader("Base.frag", Shader::Type::Static);

    m_shader[Shader::Type::StaticWavy] =
        g_ShaderMgr.getShader("Base.frag", Shader::Type::StaticWavy);

    m_shader[Shader::Type::Skinned] =
        g_ShaderMgr.getShader("Base.frag", Shader::Type::Skinned);
}

}
