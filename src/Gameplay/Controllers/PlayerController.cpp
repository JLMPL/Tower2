#include "PlayerController.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Gameplay/Entities/Creature.hpp"
#include "Gameplay/Level.hpp"
#include "Input/Input.hpp"
#include "SceneGraph/CameraNode.hpp"
#include "SceneGraph/LightNode.hpp"
#include "SceneGraph/SceneGraph.hpp"
#include "SceneGraph/SkinnedMeshNode.hpp"
#include "SceneGraph/FlareNode.hpp"

PlayerController::PlayerController(Creature* cre, LevelContext* context)
    : CreatureController(cre), m_context(context)
{
    enterIdle();

    m_camera = m_context->sceneGraph->addCameraNode()->as<CameraNode>();
    m_camera->setOffset(vec3(-0.75, 2, -2));
    m_cameraHolder = m_context->sceneGraph->addEmptyNode();

    m_cameraHolder->attachNode(m_camera);
    m_context->sceneGraph->getRoot()->attachNode(m_cameraHolder);

    m_sord = m_context->sceneGraph->addMeshNode("key.obj");
    m_sord->setPosition(vec3(0,0.5,0));
    m_cre->getSkinMeshNode()->attachNode("Hand.R", m_sord);


    m_light = m_context->sceneGraph->addFlareNode("flare.png")->as<FlareNode>();
    m_light->setColor(Color(0,1,0,1));
    m_light->setScale(0.75);
    m_light->setPosition(vec3(0,0.25,0));

    m_cre->getSkinMeshNode()->attachNode("Hand.R", m_light);

    // m_sord->attachNode(m_light);
    // m_cre->getSkinMeshNode()->attachNode("Hand.R", m_light);
}

void PlayerController::onEvent(const GameEvent& event)
{
}

void PlayerController::update()
{
    m_interactible = m_context->level->getClosestInteractible(m_cre->getPos(), m_cre->getFacingDir());

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

    m_cameraHolder->setPosition(math::lerp(m_cameraHolder->getPosition(), m_cre->getPos(), 0.4f));

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
        // m_cre->getAnimator().setState("Draw");
        m_cre->drawSword();
    }
    else if (gInput.isDrawMelee() && m_cre->isSwordDrawn() && idleAnim == currAnim)
    {
        // m_cre->getAnimator().setState("Sheathe");
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

    vec2 rightAxis = gInput.getRightAxis();

    if (m_cre->isSwordDrawn())
    {
        if (gInput.isAttack())
            enterAttack();
    }

    {
        // m_cre->setFacingDirection(math::rotateY(m_cre->getFacingDir(), -rightAxis.x * 0.0025f));

        m_cameraHolderYaw -= rightAxis.x * 0.0025f;
        m_cameraHolder->setRotation(math::rotate(quat(), m_cameraHolderYaw, math::vecY));

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
    vec2 rightAxis = gInput.getRightAxis();
    vec2 leftAxis = gInput.getLeftAxis();

    // if (m_cre->isSwordDrawn())
    // {
    //     auto cbTarget = m_context->level->getClosestCombatTarget(m_cre->getPos(), m_cre->getFacingDir());
    // }

    m_cameraHolderYaw -= rightAxis.x * 0.0025f;
    m_cameraHolder->setRotation(math::rotate(quat(), m_cameraHolderYaw, math::vecY));

    vec3 oldir = m_cre->getFacingDir();
    vec3 cameraForward = m_camera->as<CameraNode>()->getForwardDirection();

    m_cre->setFacingDirection(math::normalize(math::lerp(oldir, cameraForward, 0.2f)));

    vec3 godir;
    if (leftAxis.y > 0)
    {
        godir += cameraForward;
        m_cre->getAnimator().setState("Walk");
    }
    if (leftAxis.y < 0)
    {
        godir += -cameraForward;
        m_cre->getAnimator().setState("Walk");
    }
    if (leftAxis.x > 0)
    {
        godir += math::rotateY(cameraForward, f32(-HALF_PI));
        m_cre->getAnimator().setState("WalkRight");
    }
    if (leftAxis.x < 0)
    {
        godir += math::rotateY(cameraForward, f32(HALF_PI));
        m_cre->getAnimator().setState("WalkLeft");
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

    // m_cre->getAnimator().setState("Attack");
    // m_cre->getAnimator().callFunctionOnGlobalTime(
    // [&]()
    // {
    //     enterIdle();
    // }, core::g_FInfo.globalTime + m_cre->getAnimator().getState("Attack")->getDuration());
}

void PlayerController::attack()
{
    auto cbTarget = m_context->level->getClosestCombatTarget(m_cre->getPos(), m_cre->getFacingDir());

    if (cbTarget)
    {
        vec3 oldir = m_cre->getFacingDir();
        vec3 nudir = math::normalize(cbTarget->getPos() - m_cre->getPos());

        m_cre->setFacingDirection(math::normalize(math::lerp(oldir, nudir, 0.2f)));
    }

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