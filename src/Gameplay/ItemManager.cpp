#include "ItemManager.hpp"
#include <Debug/Log.hpp>
#include <fstream>

ItemManager g_ItemMgr;

void ItemManager::loadItems()
{
    loadItemFile("Items/Consumables.itm");
    loadItemFile("Items/Keys.itm");
}

void ItemManager::loadItemFile(const std::string& path)
{
    std::ifstream file(path);
    json items;

    try
    {
        file >> items;
    }
    catch (nlohmann::detail::exception)
    {
        printf("Could not parse json in %s\n", path.c_str());
    }

    for (u32 i = 0; i < items["items"].size(); i++)
    {
        m_items.emplace_back();
        m_items.back().load(items["items"][i]);
    }
}

const Item* ItemManager::getItem(u32 id) const
{
    for (auto& i : m_items)
    {
        if (i.m_id == id)
            return &i;
    }

    Log::warning("No such item exists id(%d)\n", id);
    return nullptr;
}
