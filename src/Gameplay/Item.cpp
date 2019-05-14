#include "Item.hpp"
#include "Render/MeshManager.hpp"
#include "Render/TextureManager.hpp"
#include "Render/MaterialManager.hpp"

Item::Type Item::getTypeFromString(const std::string& type)
{
    if (type == "food")
    {
        return Item::Type::Food;
    }
    else if (type == "key")
    {
        return Item::Type::Key;
    }
}

void Item::load(json& node)
{
    if (node["id"].is_null())
    {
        Log::warning("Item description does not contain id!\n");
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
    m_type = getTypeFromString(node["type"].get<std::string>());
    m_name = node["name"].get<std::string>();

    m_mesh = gfx::g_MeshMgr.getMesh(node["mesh"].get<std::string>());
    m_material = gfx::g_MatMgr.getMaterial(node["texture"].get<std::string>());
    m_image = gfx::g_TexMgr.getTexture(node["image"].get<std::string>());

    m_consumable = true;

    if (!node["damage"].is_null())
        m_damage = node["damage"].get<i32>();

    if (!node["defense"].is_null())
        m_defense = node["defense"].get<i32>();
}
