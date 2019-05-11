#pragma once
#include "Core/Timer.hpp"
#include "Gameplay/Entity.hpp"

class SceneGraph;
class SceneNode;
struct GameEvent;

class LightEffect final : public Entity
{
public:
    LightEffect(u32 id, LevelContext* context);
    void update() override final;

    void onEvent(const GameEvent& event) {}

    Type getType() const override final;

private:
    SceneNode* m_light = nullptr;
    SceneNode* m_flare = nullptr;

    core::Timer m_timer;
    vec3 m_posOffset = vec3(1,0,0);
    f32 m_angle = 0.f;
};
