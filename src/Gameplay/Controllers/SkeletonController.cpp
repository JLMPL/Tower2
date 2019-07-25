#include "SkeletonController.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Gameplay/Entities/Creature.hpp"
#include "Gameplay/Level.hpp"
#include "Input/Input.hpp"
#include "Render/Scene/RenderLight.hpp"
#include "Render/Scene/RenderScene.hpp"
#include "Render/Scene/RenderMesh.hpp"
#include "Render/Scene/RenderFlare.hpp"
#include "Render/Scene/RenderSkinnedMesh.hpp"
#include "Animation/Animation.hpp"

SkeletonController::SkeletonController(Creature* cre, LevelContext* context)
    : CreatureController(cre), m_context(context)
{
    enterIdle();

    m_sord = m_context->renderScene->addRenderMesh("sord.obj");
    // m_sord->setPosition(vec3(0,0.25,0));
    // m_cre->getSkinMeshNode()->attachNodeToJoint("Hand.R", m_sord);

    m_light = m_context->renderScene->addRenderFlare("flare.png");
    m_light->setColor(Color(1,0,0,1));
    m_light->setScale(5.f);
    m_light->setPosition(vec3(0,0.25,0));
    m_light->hide();

    // m_cre->getSkinMeshNode()->attachNodeToJoint("Hand.R", m_light);

    // m_sord->attachNode(m_light);
    // m_cre->getSkinMeshNode()->attachNode("Hand.R", m_light);

    auto animator = &m_cre->getAnimator();

    anim::bindAnimStateEvent(anim::getAnimatorState(animator, "Attack0"), 0.333333,
    [&]()
    {
        GameEvent event(GameEvent::Type::Damage);
        event.ivalue = 1;
        m_context->eventSys->enqueue(event);
    });

    anim::bindAnimStateEvent(anim::getAnimatorState(animator, "Attack0"), 0.625,
    [&]()
    {
        enterIdle();
    });

    anim::bindAnimStateEvent(anim::getAnimatorState(animator, "Backflip"), anim::getAnimatorState(animator, "Backflip")->anim->duration,
    [&]()
    {
        enterIdle();
    });

    anim::bindAnimStateEvent(anim::getAnimatorState(animator, "Pain"), anim::getAnimatorState(animator, "Pain")->anim->duration,
    [&]()
    {
        enterIdle();
    });
}

void SkeletonController::onEvent(const GameEvent& event)
{
    if (event.getReceiver() == m_cre->getID())
    {
        switch (event.getType())
        {
            case GameEvent::Type::Damage:
                enterPain();
                break;
        }
    }
}

void SkeletonController::update()
{
    switch (m_state)
    {
        case State::Idle:
            idle();
            break;
        case State::Move:
            move();
            break;
        case State::Attack:
            attack();
            break;
        case State::Pain:
            pain();
            break;
        case State::Death:
            death();
            break;
    }
}

void SkeletonController::enterIdle()
{
    m_state = State::Idle;
    anim::setAnimatorState(&m_cre->getAnimator(), "Idle");
}

void SkeletonController::idle()
{
    if (m_cre->isDead())
    {
        // enterDead();
        return;
    }

    fall();
}

void SkeletonController::enterMove()
{
    m_state = State::Move;
}

void SkeletonController::move()
{

}

void SkeletonController::enterAttack()
{
    m_state = State::Attack;

    if (m_combo == 0)
        anim::setAnimatorState(&m_cre->getAnimator(), "Attack0");
    else
        anim::setAnimatorState(&m_cre->getAnimator(), "Backflip");
}

void SkeletonController::attack()
{
    auto cbTarget = m_context->level->getClosestCombatTarget(m_cre->getPos(), m_cre->getFacingDir());

    if (cbTarget)
    {
        m_combatTarget = cbTarget->getID();
        vec3 oldir = m_cre->getFacingDir();
        vec3 nudir = math::normalize(cbTarget->getPos() - m_cre->getPos());

        m_cre->setFacingDirection(math::normalize(math::lerp(oldir, nudir, 0.2f)));
    }
    else
        m_combatTarget = -1;

    m_cre->setDirection(m_cre->getFacingDir());

}

void SkeletonController::enterPain()
{
    m_state = State::Pain;
    m_cre->damage(1);

    if (!m_cre->isDead())
        anim::setAnimatorState(&m_cre->getAnimator(), "Pain");
    else
        enterDeath();
}

void SkeletonController::pain()
{

}

void SkeletonController::enterDeath()
{
    m_state = State::Death;

    anim::setAnimatorState(&m_cre->getAnimator(), "Death");

    // GameEvent event(GameEvent::Type::SpawnPickup);
    // event.pickup.x = m_cre->getPos().x;
    // event.pickup.y = m_cre->getPos().y + 2;
    // event.pickup.z = m_cre->getPos().z;
    // event.pickup.itemID = 0;

    // m_context->eventSys->enqueue(event);

    for (auto i = 0; i < 10; i++)
    {
        m_context->level->addPickup(0, m_cre->getPos() + vec3(0,2,0));
    }
}

void SkeletonController::death()
{

}

void SkeletonController::fall()
{
    m_cre->getCharCtrl().move(vec3(0,-1,0), core::g_FInfo.delta);
}
