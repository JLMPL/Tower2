#pragma once
#include "Shader.hpp"
#include "Core/Json.hpp"

namespace gfx
{

class Texture;

class Material
{
public:
    void load(std::json& node);

public:
    const Texture*    m_textures[8u] = {nullptr};
    u32               m_numTextures = 0u;
};

}
