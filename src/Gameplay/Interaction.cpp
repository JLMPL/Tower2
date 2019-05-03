#include "Interaction.hpp"
#include "Core/FrameInfo.hpp"
#include "Creature.hpp"

void Interaction::begin(Creature* creature, Interactible* entity)
{
    if (m_finished)
        return;

    m_creature = creature;
    m_entity = entity;

    m_stage = Stage::LerpIn;
    m_requirementsMet = false;
    m_lerp = 0;
    m_isRunning = true;

    m_creatureLerpPos[0] = m_creature->getPos();
    m_creatureLerpDir[0] = m_creature->getFacingDir();
}

void Interaction::lerpIn()
{
    m_lerp += core::g_FInfo.delta *3;

    if (m_lerp >= 1)
    {
        m_stage = Stage::PlayAnim;
        m_creature->getAnimator().setState(m_anim);
        m_creature->getAnimator().callFunctionOnGlobalTime(
        [&]()
        {
            m_stage = Stage::LerpOut;
            m_lerp = 0;
            m_creature->getAnimator().setState("Idle");
        },
        core::g_FInfo.globalTime + m_creature->getAnimator().getState(m_anim)->getDuration());
        m_lerp = 1;
    }

    m_creature->setPos(math::lerp(m_creatureLerpPos[0], m_creatureLerpPos[1], m_lerp));
    m_creature->setFacingDirection(math::normalize(math::lerp(m_creatureLerpDir[0], m_creatureLerpDir[1], m_lerp)));
}

void Interaction::playAnim()
{

}

void Interaction::lerpOut()
{
    m_lerp += core::g_FInfo.delta *3;

    if (m_lerp >= 1)
    {
        m_finishFunc(m_creature);
        m_finished = true;
        m_isRunning = false;

        m_lerp = 1;
    }
}

void Interaction::update()
{
    if (m_finished)
    {
        m_isRunning = false;
        return;
    }

    if (!m_requirementsMet)
    {
        if (!m_requirementsFunc())
        {
            m_isRunning = false;
            return;
        }

        m_requirementsMet = true;
    }

    switch (m_stage)
    {
        case Stage::LerpIn:
            lerpIn();
            break;
        case Stage::PlayAnim:
            playAnim();
            break;
        case Stage::LerpOut:
            lerpOut();
            break;
        default: break;
    }
}

void Interaction::setRequirementFunc(const std::function<bool (void)>& func)
{
    m_requirementsFunc = func;
}

void Interaction::setFinishFunc(const std::function<void (Creature*)>& func)
{
    m_finishFunc = func;
}

void Interaction::setAnimation(const std::string& anim)
{
    m_anim = anim;
}

void Interaction::setCreaturePosition(const vec3& pos, const vec3& dir)
{
    m_creatureLerpPos[1] = pos;
    m_creatureLerpDir[1] = dir;
}

bool Interaction::isRunning() const
{
    return m_isRunning;
}
