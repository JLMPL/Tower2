#pragma once
#include "Core/Types.hpp"

class Creature;

class GameEvent
{
public:
    enum Type
    {
        Invalid = 0,
        UpdateHud,
        Damage,
        SpawnPickup
    };

    GameEvent() = default;
    GameEvent(Type type) : m_type(type) {}
    ~GameEvent() = default;

    void setSender(i32 id);
    void setReceiver(i32 id);

    i32 getSender() const;
    i32 getReceiver() const;

    Type getType() const;

public:
    struct UpdateHudData
    {
        u32 health = 0;
        u32 mana = 0;
        u8 spell = 0;
        f32 focusX = 0;
        f32 focusY = 0;
        f32 focusZ = 0;
        bool focus = false;
    };

    struct SpawnPickupData
    {
        f32 x,y,z;
        i32 itemID;
    };

    union
    {
        f32 fvalue;
        i32 ivalue;

        UpdateHudData hud;
        SpawnPickupData pickup;
    };

private:
    Type m_type = Type::Invalid;

    i32 m_sender = -1;
    i32 m_receiver = -1;
};
