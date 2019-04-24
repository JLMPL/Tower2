#include "Arrow.hpp"
#include "Core/FrameInfo.hpp"
#include "Render/MeshManager.hpp"
#include "Render/MaterialManager.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "EventSystem/EventSystem.hpp"

Arrow::Arrow(const vec3& pos, const vec3& dir, f32 speed)
{
    init(pos, dir, speed);
}

void Arrow::init(const vec3& pos, const vec3& dir, f32 speed)
{
    m_pos = pos;
    m_dir = dir;
    m_speed = speed;

    // m_mesh = gfx::g_Renderer3D.addStatic(
        // &gfx::g_MeshMgr.getMesh("arrow.obj")->entries[0], gfx::g_MatMgr.getMaterial("env_wood"));
}

void Arrow::move()
{
    m_pos += m_dir * m_speed * core::g_FInfo.delta;
}

void Arrow::updateTransforms()
{
    vec3 flatDir = math::normalize(vec3(m_dir.x, 0, m_dir.z));
    f32 yaw = atan2(flatDir.x, flatDir.z);

    f32 fDotD = math::dot(flatDir, m_dir);
    f32 pitch = acos(fDotD);

    f32 adjust = (m_dir.y >= 0) ? -pitch : pitch;

    m_transform =
        math::translate(m_pos) *
        math::rotate(yaw, vec3(0,1,0)) *
        math::rotate(HALF_PI + adjust, vec3(1,0,0));
}

void Arrow::checkCollisions()
{
    auto result = phys::g_PhysSys.raycast(m_pos, m_dir, 0.5);

    if (result.hasHit)
    {
        // m_dir = math::reflect(m_dir, result.normal);
        m_speed = 0;

        if (result.entityID != -1)
        {
            GameEvent event(GameEvent::CreatureRelated, GameEvent::Creature_Damaged);
            event.setReceiver(result.entityID);
            g_EventSys.enqueue(event);

            destroy();
        }
    }
}

void Arrow::update()
{
    move();
    updateTransforms();
    checkCollisions();

    // gRenderer3D.addLine(m_pos, m_pos + m_dir, vec3(1));
    // m_mesh.setTransform(m_transform);

}

Projectile::Type Arrow::getType() const
{
    return Type::Arrow;
}
