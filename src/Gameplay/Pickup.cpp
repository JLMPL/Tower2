#include "Pickup.hpp"
#include "Creature.hpp"
#include "ItemManager.hpp"

void Pickup::init(u32 id, const std::string& code)
{
    Entity::init(id, code);

    m_item = g_ItemMgr.getItem("damn_herb");
    m_labelName = m_item->m_name;

    // m_mesh = gfx::g_Renderer3D.addFoliage(&m_item->m_mesh->entries[0], m_item->m_material);
}

void Pickup::addRigidBody(const vec3& pos)
{
    m_hasRigidBody = true;
    m_rigidBody = phys::g_PhysSys.addBox(pos, vec3(0), vec3(0.1));
}

void Pickup::addLight()
{
    m_hasLight = true;
    // m_light.color = vec3(1,0.8,0.01);
    // m_light.intensity = 10.f;
    // m_light.enabled = true;

    // gfx::g_Renderer3D.addLight(&m_light);
}

void Pickup::update()
{
    if (m_hasRigidBody)
    {
        m_transform = m_rigidBody.getGlobalTransform();// * math::rotate(-HALF_PI, vec3(0,0,1));
        m_pos = m_transform[3];
    }
    else
        m_transform = math::translate(m_pos);

    // m_light.pos = m_pos;

    vec4 linez[4] =
    {
        vec4(0,0,0,1),
        vec4(1,0,0,0),
        vec4(0,1,0,0),
        vec4(0,0,1,0)
    };

    for (u32 i = 0; i < 4; i++)
        linez[i] = m_rigidBody.getGlobalTransform() * linez[i];

    // gfx::g_Renderer3D.addLine(linez[0], linez[0] + linez[1], vec3(1,0,0));
    // gfx::g_Renderer3D.addLine(linez[0], linez[0] + linez[2], vec3(0,1,0));
    // gfx::g_Renderer3D.addLine(linez[0], linez[0] + linez[3], vec3(0,0,1));

    // m_mesh.setTransform(m_transform);
}

void Pickup::interact(Creature* other)
{
    other->getAnimator().setState("Pickup");
    other->getAnimator().clearPendingFunctions();
    other->getAnimator().callFunctionOnGlobalTime(
    [=]()
    {
        other->getEquipment()->give(this->m_item, 1);

        if (this->m_item->m_type == Item::Type::Torch && !other->getEquipment()->hasEquippedTorch())
        {
            // other->tryEquipItem(this->m_item);
        }
        this->disable();
    }, core::g_FInfo.globalTime + other->getAnimator().getState("Pickup")->getDuration()/2);
}

void Pickup::disable()
{
    Base::disable();

    if (m_hasRigidBody)
        m_rigidBody.disable();

    if (m_hasLight)
    {
        // m_light.enabled = false;
        // gfx::g_Renderer3D.removeLight(&m_light);
    }

    phys::g_PhysSys.removeRigidBody(m_rigidBody);
    m_hasRigidBody = false;

    // m_mesh.hide();

    Base::destroy();
}

phys::RigidBody& Pickup::getRigidBody()
{
    return m_rigidBody;
}

Entity::Type Pickup::getType() const
{
    return Type::Pickup;
}

