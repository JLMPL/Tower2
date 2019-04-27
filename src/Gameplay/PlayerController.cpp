#include "PlayerController.hpp"
#include "Input/Input.hpp"
#include "Creature.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Level.hpp"

PlayerController::PlayerController(Creature* cre)
    : CreatureController(cre)
{
    enterIdle();
}

void PlayerController::onEvent(const GameEvent& event)
{
}

void PlayerController::update()
{
    m_interactible = m_cre->getLevel()->getClosestInteractible(m_cre->getPos(), m_cre->getFacingDir());

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
    }

/*    if (m_cre->getAnimator().isRootMotion())
    {
        vec3 dir = math::rotateY(m_cre->getAnimator().getRootMotion(), m_cre->getYaw());

        m_cre->getCharCtrl().move(dir);
    }*/
}

void PlayerController::checkDrawWeapon()
{
    auto idleAnim = m_cre->getAnimator().getState("Idle");
    auto currAnim = m_cre->getAnimator().getCurrentState();

    if (gInput.isDrawMelee() && !m_cre->isSwordDrawn() && idleAnim == currAnim)
    {
        m_cre->getAnimator().setState("Draw");
        m_cre->drawSword();
    }
    else if (gInput.isDrawMelee() && m_cre->isSwordDrawn() && idleAnim == currAnim)
    {
        m_cre->getAnimator().setState("Sheathe");
        m_cre->sheatheSword();
    }
}

void PlayerController::enterIdle()
{
    m_state = State::Idle;
    m_cre->getAnimator().setState("Idle");
}

void PlayerController::idle()
{
    if (m_cre->isDead())
    {
        // enterDead();
        return;
    }

    if (m_cre->isBusy())
        return;

    vec2 rightAxis = gInput.getRightAxis();

    if (m_cre->isSwordDrawn())
    {
        auto cbTarget = m_cre->getLevel()->getClosestCombatTarget(m_cre->getPos(), vec3(0));

        if (!cbTarget)
        {
            m_cre->setFacingDirection(math::rotateY(m_cre->getFacingDir(), -rightAxis.x * 0.0025f));
        }
        else
        {
            vec3 oldir = m_cre->getFacingDir();
            vec3 nedir = math::normalize(cbTarget->getPos() - m_cre->getPos());

            m_cre->setFacingDirection(math::normalize(math::lerp(oldir, nedir, 0.2f)));
        }

        if (gInput.isAttack())
            enterAttack();
    }
    else
    {
        m_cre->setFacingDirection(math::rotateY(m_cre->getFacingDir(), -rightAxis.x * 0.0025f));

        if (gInput.isUse() && m_interactible)
        {
            m_interactible->interact(m_cre);
        }
    }

    checkDrawWeapon();

    if (!m_cre->getCharCtrl().isOnGround())
        fall();

    if (isAnyMovementKey())
        enterMove();
}

void PlayerController::enterMove()
{
    m_state = State::Move;
}

void PlayerController::move()
{
    if (m_cre->isBusy())
        return;

    vec2 rightAxis = gInput.getRightAxis();
    vec2 leftAxis = gInput.getLeftAxis();

    if (m_cre->isSwordDrawn())
    {
        auto cbTarget = m_cre->getLevel()->getClosestCombatTarget(m_cre->getPos(), m_cre->getFacingDir());

        if (!cbTarget)
        {
            m_cre->setFacingDirection(math::rotateY(m_cre->getFacingDir(), -rightAxis.x * 0.0025f));
        }
        else
        {
            vec3 oldir = m_cre->getFacingDir();
            vec3 nedir = math::normalize(cbTarget->getPos() - m_cre->getPos());

            m_cre->setFacingDirection(math::normalize(math::lerp(oldir, nedir, 0.2f)));
        }
    }
    else
    {
        m_cre->setFacingDirection(math::rotateY(m_cre->getFacingDir(), -rightAxis.x * 0.0025f));
    }

    vec3 godir;
    if (leftAxis.y > 0)
    {
        godir += m_cre->getFacingDir();
        m_cre->getAnimator().setState("Run");
    }
    if (leftAxis.y < 0)
    {
        godir += -m_cre->getFacingDir();
        m_cre->getAnimator().setState("Run");
    }
    if (leftAxis.x > 0)
    {
        godir += math::rotateY(m_cre->getFacingDir(), f32(-HALF_PI));
        m_cre->getAnimator().setState("RunRight");
    }
    if (leftAxis.x < 0)
    {
        godir += math::rotateY(m_cre->getFacingDir(), f32(HALF_PI));
        m_cre->getAnimator().setState("RunLeft");
    }

    m_cre->setDirection(math::normalize(godir));
    m_cre->getCharCtrl().move(m_cre->getDirection() * 5.f, core::g_FInfo.delta);

    if (gInput.isAttack())
        enterAttack();

    // if (isJumpKey())
        // m_stateMachine->setState(new PlayerJump());

    if (!m_cre->getCharCtrl().isOnGround())
        fall();

    if (!isAnyMovementKey())
        enterIdle();
}

void PlayerController::enterAttack()
{
    m_state = State::Attack;

    m_cre->getAnimator().setState("Attack");
    m_cre->getAnimator().callFunctionOnGlobalTime(
    [&]()
    {
        enterIdle();
    }, core::g_FInfo.globalTime + m_cre->getAnimator().getState("Attack")->getDuration());

    m_cre->getAnimator().callFunctionOnGlobalTime(
    [&]()
    {
    }, core::g_FInfo.globalTime + (m_cre->getAnimator().getState("Attack")->getDuration() /2));
}

void PlayerController::attack()
{
    if (m_cre->isBusy())
        return;

    if (!m_cre->getFocus())
        return;

    m_cre->setDirection(m_cre->getFacingDir());
}

void PlayerController::fall()
{
    m_cre->getCharCtrl().move(vec3(0,-1,0), core::g_FInfo.delta);
}

bool PlayerController::isAnyMovementKey() const
{
    return gInput.getLeftAxis() != vec2(0);
}
