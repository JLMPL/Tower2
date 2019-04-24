#pragma once
#include "Core/Types.hpp"

#define SET_BIT(x) (1 << x)

class Creature;

class GameEvent
{
public:
    enum Category
    {
        Null            = 0,
        PlayerRelated   = SET_BIT(0),
        CreatureRelated = SET_BIT(1),
        EntityRelated   = SET_BIT(2),
        SystemRelated   = SET_BIT(3)
    };

    enum Type
    {
        Invalid = 0,

        Player_WeaponDrawn,
        Player_WeaponSheathed,
        Player_GainCombatFocus,
        Player_LoseCombatFocus,
        Player_Hit,
        Player_BlockInput,
        Player_AllowInput,
        Player_BowDrawn,
        Player_BowReleased,

        Creature_Damaged,
        Creature_Killed,

        Entity_Activated
    };

    GameEvent() = default;
    GameEvent(Category cat, Type type) :
        m_category(cat), m_type(type)
    {}
    ~GameEvent() = default;

    void setSender(i32 id);
    void setReceiver(i32 id);

    i32 getSender() const;
    i32 getReceiver() const;

    bool isInCategory(Category catg) const;
    Type getType() const;

public:
    Creature* creature = nullptr;
    f32 fvalue = 0;
    i32 ivalue = 0;

private:
    Category m_category = Category::Null;
    Type m_type = Type::Invalid;

    i32 m_sender = -1;
    i32 m_receiver = -1;
};
