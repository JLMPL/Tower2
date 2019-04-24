#pragma once
#include "Shader.hpp"
#include "Core/Json.hpp"

namespace gfx
{

class Texture;
class Resources;

class Material
{
public:
    enum Blending
    {
        None,
        Transparent,
        Additive
    };

    void load(json& node);

public:
    Blending          m_blending = Blending::None;

    const Texture*    m_textures[8u] = {nullptr};
    u32               m_numTextures = 0u;

    bool              m_useTime = false;
    bool              m_usePointLights = false;
    bool              m_useGlobalLight = false;

    const Shader*     m_shader[Shader::Type::Count] = {nullptr};
};

}
