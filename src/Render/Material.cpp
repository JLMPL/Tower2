#include "Material.hpp"

namespace gfx
{

Material::Material(const std::string& name)
{
    std::string d = name + "_d";
    m_diffuse = TextureCache.load<TextureLoader>(entt::hashed_string{d.c_str()}, name + "/diffuse.png");
    std::string n = name + "_n";
    m_normal = TextureCache.load<TextureLoader>(entt::hashed_string{n.c_str()}, name + "/normal.png");
    std::string s = name + "_s";
    m_specular = TextureCache.load<TextureLoader>(entt::hashed_string{s.c_str()}, name + "/specular.png");
}

}
