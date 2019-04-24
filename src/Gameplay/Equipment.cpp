#include "Equipment.hpp"
#include "Item.hpp"
#include "ItemManager.hpp"
#include <Debug/Log.hpp>

void Equipment::init(u32 id)
{
    m_ownerId = id;
}

void Equipment::give(const Item* item, u32 count)
{
    for (auto& i : m_items)
    {
        if (i.item == item)
        {
            i.count += count;
            return;
        }
    }

    m_items.push_back({item, count});
}

void Equipment::give(const Code& item, u32 count)
{
    for (auto& i : m_items)
    {
        if (i.item->m_code == item)
        {
            i.count += count;
            return;
        }
    }

    m_items.push_back({g_ItemMgr.getItem(item), count});
}

void Equipment::take(const Item* item, u32 count)
{
    for (auto& i : m_items)
    {
        if (i.item == item)
        {
            if (i.count >= count)
                i.count -= count;
            else
                i.count = 0;
        }
    }

    for (auto i = m_items.begin(); i != m_items.end();)
    {
        if ((*i).count == 0)
            i = m_items.erase(i);
        else
            i++;
    }
}

void Equipment::take(const Code& item, u32 count)
{
    for (auto& i : m_items)
    {
        if (i.item->m_code == item)
        {
            if (i.count >= count)
                i.count -= count;
            else
                i.count = 0;
        }
    }

    for (auto i = m_items.begin(); i != m_items.end();)
    {
        if ((*i).count == 0)
            i = m_items.erase(i);
        else
            i++;
    }
}

bool Equipment::hasItem(const Item* item) const
{
    for (auto& i : m_items)
    {
        if (i.item == item &&
            i.count > 0)
            return true;
    }

    return false;
}

bool Equipment::hasItem(const Code& item)
{
    for (auto& i : m_items)
    {
        if (i.item->m_code == item &&
            i.count > 0)
            return true;
    }

    return false;
}

const Item* Equipment::hasWeapon() const
{
    for (auto& i : m_items)
    {
        if (i.item->m_type == Item::Type::Weapon)
            return i.item;
    }

    return nullptr;
}

void Equipment::equip(const Item* item)
{
    if (!hasItem(item))
    {
        Log::warning("Creature %d: Can't equip \"%s\" - don't have it!", m_ownerId, item->m_name.c_str());
        return;
    }

    switch (item->m_type)
    {
        case Item::Type::Weapon:
        {
            if (item->m_weaponType == Item::WeaponType::Sword)
                m_slots[Slot::Melee] = item;
            else
                m_slots[Slot::Ranged] = item;
            printf("Creature %d: Equipped weapon %s\n", m_ownerId, item->m_name.c_str());
        }
        break;
        case Item::Type::Torch:
            m_slots[Slot::Torch] = item;
            printf("Creature %d: Equipped torch %s\n", m_ownerId, item->m_name.c_str());
        break;
        default: break;
    }
}

void Equipment::unequip(const Item* item)
{
    if (!hasItem(item))
    {
        Log::warning("Creature %d: Can't Unequip \"%s\" - don't have it!", m_ownerId, item->m_name.c_str());
        return;
    }

    switch (item->m_type)
    {
        case Item::Type::Weapon:
        {
            if (item->m_weaponType == Item::WeaponType::Sword)
                m_slots[Slot::Melee] = nullptr;
            else
                m_slots[Slot::Ranged] = nullptr;
            printf("Creature %d: Unquipped item %s\n", m_ownerId, item->m_name.c_str());
        }
        break;
        case Item::Type::Torch:
            m_slots[Slot::Torch] = nullptr;
            printf("Creature %d: Unquipped torch %s\n", m_ownerId, item->m_name.c_str());
        break;
        default: break;
    }
}

bool Equipment::isItemEquipped(const Item* item) const
{
    if (m_slots[Slot::Melee] == item)
        return true;

    if (m_slots[Slot::Ranged] == item)
        return true;

    if (m_slots[Slot::Torch] == item)
        return true;

    return false;
}

const Item* Equipment::getItemInSlot(Slot slot) const
{
    return m_slots[slot];
}

const Item* Equipment::getEquippedMelee() const
{
    return m_slots[Slot::Melee];
}

const Item* Equipment::getEquippedRanged() const
{
    return m_slots[Slot::Ranged];
}

const Item* Equipment::getEquippedTorch() const
{
    return m_slots[Slot::Torch];
}

bool Equipment::hasEquippedTorch() const
{
    return m_slots[Slot::Torch];
}

u32 Equipment::getNumItems() const
{
    return m_items.size();
}

const Item* Equipment::getItemAtIndex(u32 i) const
{
    return m_items[i].item;
}
