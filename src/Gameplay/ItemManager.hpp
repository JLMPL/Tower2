#pragma once
#include "Gameplay/Item.hpp"
#include <vector>

class ItemManager
{
public:
    void loadItems();

    const Item* getItem(u32 id) const;
    const Item* getItem(const std::string& code) const;

private:
    void loadItemFile(const std::string& path);

private:
    std::vector<Item> m_items;
};

extern ItemManager g_ItemMgr;
