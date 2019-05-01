#include "Item.hpp"
#include "Render/MeshManager.hpp"
#include "Render/TextureManager.hpp"
#include "Render/MaterialManager.hpp"

Item::Type Item::getTypeFromString(const String& type)
{
    if (type == "food")
    {
        return Item::Type::Food;
    }
    else if (type == "one_handed")
    {
        m_weaponType = WeaponType::Sword;
        return Item::Type::Weapon;
    }
    else if (type == "bow")
    {
        m_weaponType = WeaponType::Bow;
        return Item::Type::Weapon;
    }
    else if (type == "torch")
    {
        return Item::Type::Torch;
    }
    else if (type == "key")
    {
        return Item::Type::Key;
    }
    else if (type == "armor")
    {
        return Item::Type::Armor;
    }
}

void Item::load(json& node)
{
    if (node["id"].is_null())
    {
        Log::warning("Item description does not contain id!\n");
        return;
    }

    if (node["code"].is_null())
    {
        Log::warning("Item description does not contain code!\n");
        return;
    }

    if (node["name"].is_null())
    {
        Log::warning("Item description does not contain name!\n");
        return;
    }

    if (node["mesh"].is_null())
    {
        Log::warning("Item description does not contain mesh!\n");
        return;
    }

    if (node["texture"].is_null())
    {
        Log::warning("Item description does not contain texture!\n");
        return;
    }

    if (node["image"].is_null())
    {
        Log::warning("Item description does not contain image!\n");
        return;
    }

    m_id = node["id"].get<i32>();
    m_code = node["code"].get<String>();
    m_type = getTypeFromString(node["type"].get<String>());
    m_name = node["name"].get<String>();

    m_mesh = gfx::g_MeshMgr.getMesh(node["mesh"].get<String>());
    m_material = gfx::g_MatMgr.getMaterial(node["texture"].get<String>());
    m_image = gfx::g_TexMgr.getTexture(node["image"].get<String>());

    // if (!node["consumable"].is_null())
        m_consumable = true;

    // Item specific

    if (!node["damage"].is_null())
        m_damage = node["damage"].get<i32>();

    if (!node["defense"].is_null())
        m_defense = node["defense"].get<i32>();
}
