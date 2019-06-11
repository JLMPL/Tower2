#include "RenderSystem.hpp"
#include "EntitySystem/Scene.hpp"
#include "EntitySystem/Components.hpp"
#include "Render/Geometry/Mesh.hpp"

namespace ecs
{

RenderSystem::RenderSystem(EntityCollection& ents) :
    System(ents)
{
    m_signature = comp::Transform::Signature | comp::StaticMesh::Signature;
}

void RenderSystem::forMatchingEntity(u32 ent)
{
    auto tr = m_ents.transforms[ent];
    auto mc = m_ents.staticMeshes[ent];

    // mc.mesh.setTransform(tr.tr);
}

}
