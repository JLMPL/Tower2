#pragma once
#include "Core/Types.hpp"
#include <vector>

struct PossessedItem
{
    const struct Item* item = nullptr;
    u32 count = 0;
};

class Equipment
{
public:
    enum Slot
    {
        Melee,
        Ranged,
        Torch,

        Count
    };

    void init(u32 id);

    void give(const Item* item, u32 count = 1);
    void give(const Code& item, u32 count = 1);
    void take(const Item* item, u32 count = 1);
    void take(const Code& item, u32 count = 1);

    bool hasItem(const Item* item) const;
    bool hasItem(const Code& item);

    const Item* hasWeapon() const;

    void equip(const Item* item);
    void unequip(const Item* item);

    bool isItemEquipped(const Item* item) const;

    const Item* getItemInSlot(Slot slot) const;

    const Item* getEquippedMelee() const;
    const Item* getEquippedRanged() const;
    const Item* getEquippedTorch() const;

    bool hasEquippedTorch() const;

    u32 getNumItems() const;
    const Item* getItemAtIndex(u32 i) const;

public:
    std::vector<PossessedItem> m_items;

    u32 m_ownerId = 0;
    const Item* m_slots[Slot::Count] = {nullptr};
};
