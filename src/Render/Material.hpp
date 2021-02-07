#pragma once
#include <string>
#include "Resource/TextureCache.hpp"

namespace gfx
{

class Material
{
public:
    Material() = default;
    Material(const std::string& name);

public:
    TextureHandle m_diffuse;
    TextureHandle m_normal;
    TextureHandle m_specular;
};

}
