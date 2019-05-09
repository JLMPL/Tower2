#pragma once
#include <Core/Json.hpp>
#include <Core/Types.hpp>

namespace gfx
{
    class Texture;
    class StaticMesh;
    class Material;
}

class Item
{
public:
    enum class Type
    {
        Weapon,
        Food,
        Key
    };

    void load(json& node);

private:
    Item::Type getTypeFromString(const std::string& type);

public:
    u32                     m_id = 0u;
    std::string             m_code = "itm_none";
    Type                    m_type;

    const gfx::Texture*     m_image = nullptr;
    const gfx::StaticMesh*  m_mesh = nullptr;
    const gfx::Material*    m_material = nullptr;

    std::string             m_name;
    std::string             m_description;

    u32                     m_value = 0u;
    bool                    m_consumable = false;

    //pertype

    u32                     m_damage = 0u;
    u32                     m_defense = 0u;
};
