#include "SkeletonController.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Gameplay/Entities/Creature.hpp"
#include "Gameplay/Level.hpp"
#include "Input/Input.hpp"
#include "SceneGraph/CameraNode.hpp"
#include "SceneGraph/LightNode.hpp"
#include "SceneGraph/SceneGraph.hpp"
#include "SceneGraph/SkinnedMeshNode.hpp"
#include "SceneGraph/FlareNode.hpp"
#include "SceneGraph/MeshNode.hpp"

SkeletonController::SkeletonController(Creature* cre, LevelContext* context)
    : CreatureController(cre), m_context(context)
{
    enterIdle();

    m_sord = m_context->sceneGraph->addMeshNode("sord.obj");
    m_sord->setPosition(vec3(0,0.25,0));
    m_cre->getSkinMeshNode()->attachNode("Hand.R", m_sord);

    m_light = m_context->sceneGraph->addFlareNode("flare.png");
    m_light->setColor(Color(1,0,0,1));
    m_light->setScale(5.f);
    m_light->setPosition(vec3(0,0.25,0));
    m_light->hide();

    m_cre->getSkinMeshNode()->attachNode("Hand.R", m_light);

    // m_sord->attachNode(m_light);
    // m_cre->getSkinMeshNode()->attachNode("Hand.R", m_light);

    auto& animator = m_cre->getAnimator();

    animator.getState("Attack0")->bindEvent(0.333333,
    [&]()
    {
        GameEvent event(GameEvent::Type::Damage);
        event.ivalue = 1;
        m_context->eventSys->enqueue(event);
    });

    animator.getState("Attack0")->bindEvent(0.625,
    [&]()
    {
        enterIdle();
    });

    animator.getState("Backflip")->bindEvent(animator.getState("Backflip")->getDuration(),
    [&]()
    {
        enterIdle();
    });

    animator.getState("Pain")->bindEvent(animator.getState("Pain")->getDuration(),
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
    m_cre->getAnimator().setState("Idle");
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
        m_cre->getAnimator().setState("Attack0");
    else
        m_cre->getAnimator().setState("Backflip");
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
        m_cre->getAnimator().setState("Pain");
    else
        enterDeath();
}

void SkeletonController::pain()
{

}

void SkeletonController::enterDeath()
{
    m_state = State::Death;

    m_cre->getAnimator().setState("Death");

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
