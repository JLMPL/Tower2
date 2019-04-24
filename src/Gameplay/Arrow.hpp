#pragma once
#include "Projectile.hpp"

class Arrow final : public Projectile
{
public:
    Arrow() = default;
    Arrow(const vec3& pos, const vec3& dir, f32 speed);

    void init(const vec3& pos, const vec3& dir, f32 speed) override final;
    void update() override final;
    Type getType() const override final;

private:
    void move();
    void updateTransforms();
    void checkCollisions();

private:
    // gfx::RenderableHandle m_mesh;

};
