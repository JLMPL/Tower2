#include "PlayerController.hpp"
#include "Animation/Skeleton.hpp"
#include "EventSystem/EventSystem.hpp"
#include "Gameplay/Entities/Creature.hpp"
#include "Gameplay/Level.hpp"
#include "Input/Input.hpp"
#include "Physics/Cloth.hpp"
#include "Render/Geometry/StaticMesh.hpp"
#include "Render/GraphRenderer.hpp"
#include "Render/MeshManager.hpp"
#include "SceneGraph/CameraNode.hpp"
#include "SceneGraph/ClothNode.hpp"
#include "SceneGraph/FlareNode.hpp"
#include "SceneGraph/LightNode.hpp"
#include "SceneGraph/MeshNode.hpp"
#include "SceneGraph/SceneGraph.hpp"
#include "SceneGraph/SkinnedMeshNode.hpp"

PlayerController::PlayerController(Creature* cre, LevelContext* context)
    : CreatureController(cre), m_context(context)
{
    enterIdle();

    m_camera = m_context->sceneGraph->addCameraNode()->as<CameraNode>();
    m_camera->setOffset(vec3(0, -0.25, -2.6));
    m_cameraHolder = m_context->sceneGraph->addEmptyNode();

    m_cameraHolder->attachNode(m_camera);
    m_context->sceneGraph->getRoot()->attachNode(m_cameraHolder);

    m_sord = m_context->sceneGraph->addMeshNode("sord.obj");
    m_sord->setPosition(vec3(0,0.25,0));
    m_cre->getSkinMeshNode()->attachNodeToJoint("Hand.R", m_sord);

    m_light = m_context->sceneGraph->addFlareNode("flare.png");
    m_light->setColor(Color(1,0,0,1));
    m_light->setScale(5.f);
    m_light->setPosition(vec3(0,0.25,0));
    m_light->hide();

    // m_cre->getSkinMeshNode()->attachNodeToJoint("Hand.R", m_light);
    // m_context->sceneGraph->getRoot()->attachNode(m_light);

    m_cape = m_context->physSys->addCloth("cape.obj");

    m_cape->setNumSpheres(6);
    m_cape->setCollisionSphere(0, vec3(0,0,0), 0.1);
    m_cape->setCollisionSphere(1, vec3(0,0,0), 0.1);
    m_cape->setCollisionSphere(2, vec3(0,0,0), 0.1);
    m_cape->setCollisionSphere(3, vec3(0,0,0), 0.1);
    m_cape->setCollisionSphere(4, vec3(0,0,0), 0.1);
    m_cape->setCollisionSphere(5, vec3(0,0,0), 0.1);
    m_cape->addCapsule(0,1);
    m_cape->addCapsule(1,2);

    m_cape->addCapsule(3,4);
    m_cape->addCapsule(4,5);

    m_capeNode = m_context->sceneGraph->addClothNode(m_cape);
    m_cre->getSkinMeshNode()->attachNode(m_capeNode);

    // m_sord->attachNode(m_light);
    // m_cre->getSkinMeshNode()->attachNode("Hand.R", m_light);

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
    m_interactible = m_context->level->getClosestInteractible(m_cre->getPos(), m_camera->getForwardDirection());

    m_spawnTimer++;

    if (m_lolo.getElapsedTime() < 0.25)
        m_light->show();
    else
        m_light->hide();

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

    m_cameraHolder->setPosition(math::lerp(m_cameraHolder->getPosition(), m_cre->getPos() + vec3(0,1.5,0), 15 * core::g_FInfo.delta));

    {
        auto& skeleton = m_cre->getAnimator().getSkeleton();
        const auto& transforms = m_cre->getAnimator().getGlobalJointTransforms();

        m_cape->setCollisionSphere(0, transforms[skeleton.findJointIndex("IK_Thigh.L")][3], 0.12);
        m_cape->setCollisionSphere(1, transforms[skeleton.findJointIndex("IK_Shin.L")][3], 0.16);
        m_cape->setCollisionSphere(2, transforms[skeleton.findJointIndex("IK_Foot.L")][3], 0.12);

        m_cape->setCollisionSphere(3, transforms[skeleton.findJointIndex("IK_Thigh.R")][3], 0.12);
        m_cape->setCollisionSphere(4, transforms[skeleton.findJointIndex("IK_Shin.R")][3], 0.16);
        m_cape->setCollisionSphere(5, transforms[skeleton.findJointIndex("IK_Foot.R")][3], 0.12);
    }
    // m_context->physSys->testo(m_cre->getTransform());
    m_cape->setTargetTransform(m_cre->getTransform());

    auto meh = m_cape->getMesh();

    std::vector<phys::Cloth::Constraint> conts;

    i8 joindex = m_cre->getAnimator().getSkeleton().findJointIndex("Root");

    mat4 as = m_cre->getAnimator().getMatrixPalette()[joindex];

    for (auto& vert : meh->entries[0].vertices)
    {
        f32 cont = (m_spawnTimer > 30) ? FLT_MAX : 0.f;

        if (vert.pos.y > 1.12)
            cont = 0.f;

        vec4 vp = math::rotate(90.0_rad, vec3(1,0,0)) * vec4(vert.pos,1);
        vec3 poz = vec3(as * vp);

        conts.push_back(phys::Cloth::Constraint(poz, cont));
    }

    m_cape->setConstraints(conts);

    // if (m_cre->getAnimator().isRootMotion())
    // {
    //     vec3 dir = math::rotateY(m_cre->getAnimator().getRootMotion(), m_cre->getYaw());

    //     m_cre->getCharCtrl().move(dir);
    // }

    updateHud();
}

void PlayerController::moveCamera()
{
    vec2 rightAxis = gInput.getRightAxis();

    m_cameraHolderYaw -= rightAxis.x * 0.0025f;
    m_cameraHolderPitch += rightAxis.y * 0.0025f;

    if (m_cameraHolderPitch < -0.6)
    {
        m_cameraHolderPitch = -0.6;
    }
    else if (m_cameraHolderPitch > 0.9)
    {
        m_cameraHolderPitch = 0.9;
    }

    m_cameraHolder->setRotation(math::rotate(quat(), m_cameraHolderYaw, math::vecY) * math::rotate(quat(), m_cameraHolderPitch, math::vecX));
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

    moveCamera();

    if (gInput.isUse() && m_interactible)
    {
        // m_interactible->interact(m_cre);
        m_cre->getAnimator().setState("Backflip");
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

    moveCamera();

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
    m_cre->getCharCtrl().move(m_cre->getDirection() * 2.5f, core::g_FInfo.delta);

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

    moveCamera();

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
