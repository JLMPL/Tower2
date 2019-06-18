#include "PlayerController.hpp"
#include "Animation/Skeleton.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Gameplay/Entities/Creature.hpp"
#include "Gameplay/Entities/Camera.hpp"
#include "Gameplay/Level.hpp"
#include "Input/Input.hpp"
#include "Render/Geometry/Mesh.hpp"
#include "Render/SceneRenderer.hpp"
#include "Render/MeshManager.hpp"
#include "Render/Scene/RenderCloth.hpp"
#include "Render/Scene/RenderFlare.hpp"
#include "Render/Scene/RenderMesh.hpp"
#include "Render/Scene/RenderScene.hpp"
#include "Render/Scene/RenderSkinnedMesh.hpp"

PlayerController::PlayerController(Creature* cre, LevelContext* context)
    : CreatureController(cre), m_context(context)
{
    enterIdle();

    m_sord = m_context->renderScene->addRenderMesh("sord.obj");

    m_light = m_context->renderScene->addRenderFlare("flare.png");
    m_light->setColor(Color(1,0,0,1));
    m_light->setScale(5.f);
    m_light->setPosition(vec3(0,0.25,0));
    m_light->hide();

    m_cape = m_context->physSys->addCloth("cape.dae", &m_cre->getAnimator());
    // m_cape2 = m_context->physSys->addCloth("cape2.dae", &m_cre->getAnimator());

    for (auto i = 0; i < 6; i++)
    {
        m_spheres[i] = phys::Cloth::Sphere({0,0,0}, 0.1);
    }

    m_cape->setCollisionSpheres(&m_spheres[0], 6);
    m_cape->addCapsule(0,1);
    m_cape->addCapsule(1,2);
    m_cape->addCapsule(3,4);
    m_cape->addCapsule(4,5);
    m_cape->addCapsule(0,3);

    m_capeNode = m_context->renderScene->addRenderCloth(m_cape);
    // m_capeNode2 = m_context->renderScene->addRenderCloth(m_cape2);

    auto& animator = m_cre->getAnimator();

    animator.getState("Attack0")->bindEvent(0.333333,
    [&]()
    {
        m_lolo.reset();

        GameEvent event(GameEvent::Type::Damage);
        event.setSender(m_cre->getID());
        event.setReceiver(m_combatTarget);
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

    animator.getState("Pickup")->bindEvent(animator.getState("Pickup")->getDuration(),
    [&]()
    {
        enterIdle();
    });

    m_dongle = m_context->physSys->addBox(vec3(0,2,0), vec3(0), vec3(0.05,0.3,0.02));
    m_context->physSys->addDistanceJoint(&m_dongle, vec3(0,0.3,0), vec3(1,2.25,0));
}

void PlayerController::onEvent(const GameEvent& event)
{
}

void PlayerController::updateHud()
{
    GameEvent event(GameEvent::Type::UpdateHud);
    event.hud.health = 1;
    event.hud.mana = 1;
    event.hud.spell = 1;

    if (m_interactible)
    {
        event.hud.focus = true;
        event.hud.focusX = m_interactible->getPos().x;
        event.hud.focusY = m_interactible->getPos().y;
        event.hud.focusZ = m_interactible->getPos().z;
    }
    else
        event.hud.focus = false;

    m_context->eventSys->enqueue(event);
}

void PlayerController::update()
{
    m_interactible = m_context->level->getClosestInteractible(m_cre->getPos(), m_context->camera->getForward());// m_camera->getForwardDirection());

    m_spawnTimer++;

    if (m_lolo.getElapsedTime() < 0.25)
        m_light->show();
    else
        m_light->hide();

    switch (m_state)
    {
        case State::Idle: idle(); break;
        case State::Move: move(); break;
        case State::Attack: attack(); break;
    }

    {
        auto& skeleton = m_cre->getAnimator().getSkeleton();
        const auto& transforms = m_cre->getAnimator().getGlobalJointTransforms();

        i32 jInds[] =
        {
            skeleton.findJointIndex("IK_Thigh.L"),
            skeleton.findJointIndex("IK_Shin.L"),
            skeleton.findJointIndex("IK_Foot.L"),

            skeleton.findJointIndex("IK_Thigh.R"),
            skeleton.findJointIndex("IK_Shin.R"),
            skeleton.findJointIndex("IK_Foot.R")
        };

        for (auto i = 0; i < 6; i++)
        {
            m_spheres[i] = phys::Cloth::Sphere(core::conv::toPx(transforms[jInds[i]][3]), 0.12);
        }

        m_cape->setCollisionSpheres(&m_spheres[0], 6);

        mat4 tr = m_cre->getTransform() * transforms[skeleton.findJointIndex("Belt0")];
        m_context->physSys->setLocalPose(tr[3]);
    }

    m_cape->setTargetTransform(m_cre->getTransform());
    m_capeNode->setTransform(m_cre->getTransform());


    m_sord->setTransform(m_dongle.getGlobalTransform() * math::translate(vec3(0,0.5,0)) * math::rotate(M_PI, vec3(1,0,0)));
    // m_cape2->setTargetTransform(m_cre->getTransform());
    // m_capeNode2->setTransform(m_cre->getTransform());

    if (m_cre->getAnimator().isRootMotion())
    {
        vec3 dir = math::rotateY(m_cre->getAnimator().getRootMotion(), m_cre->getYaw());

        m_cre->getCharCtrl().move(dir);
    }

    updateHud();
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

    if (m_cre->isSwordDrawn())
    {
        if (gInput.isAttack())
            enterAttack();
    }

    if (gInput.isUse() && m_interactible && m_cre->getAnimator().getCurrentState() == m_cre->getAnimator().getState("Idle"))
    {
        m_interactible->interact(m_cre);
    }

    checkDrawWeapon();

    if (gInput.isAttack())
    {
        enterAttack();
    }

    if (!m_cre->getCharCtrl().isOnGround())
        fall();
    else
        m_verticalVelocity = 0;

    if (isAnyMovementKey())
        enterMove();
}

void PlayerController::enterMove()
{
    m_state = State::Move;
}

void PlayerController::move()
{
    vec2 leftAxis = gInput.getLeftAxis();

    vec3 cameraForward = m_context->camera->getForward();

    f32 speed = 1.5;

    if (gInput.isJump())
    {
        m_cre->getAnimator().setState("Run");
        speed = 4;
    }
    else
        m_cre->getAnimator().setState("Walk");


    vec3 walkDir = math::normalize(
        (leftAxis.y * cameraForward) +
        (leftAxis.x * math::rotateY(cameraForward, f32(-HALF_PI)))
    );

    if (leftAxis != vec2(0))
    {
        m_cre->setDirection(walkDir);
        m_cre->setFacingDirection(math::lerp(m_cre->getFacingDir(), walkDir, core::g_FInfo.delta * 5));

        m_cre->getCharCtrl().move(m_cre->getDirection() * speed, core::g_FInfo.delta);
    }

    if (gInput.isAttack())
        enterAttack();

    if (!m_cre->getCharCtrl().isOnGround())
        fall();
    else
        m_verticalVelocity = 0;

    if (!isAnyMovementKey())
        enterIdle();
}

void PlayerController::enterAttack()
{
    m_state = State::Attack;

    if (m_combo == 0)
        m_cre->getAnimator().setState("Attack0");
    else
        m_cre->getAnimator().setState("Backflip");
}

void PlayerController::attack()
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

    if (gInput.isAttack())
    {
        if (m_lolo.getElapsedTime() < 0.25)
            m_combo++;
        else
            m_combo = 0;

        enterAttack();
    }
}

void PlayerController::fall()
{
    m_verticalVelocity -= core::g_FInfo.delta * 9.8;
    m_cre->getCharCtrl().move(vec3(0,m_verticalVelocity,0), core::g_FInfo.delta);
}

bool PlayerController::isAnyMovementKey() const
{
    return gInput.getLeftAxis() != vec2(0);
}
