#pragma once
#include "Animation/Animation.hpp"
#include "Render/Scene/RenderScene.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Camera.hpp"

union SDL_Event;

class Level
{
public:
    Level() = default;
    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;

    void initFromScript(const std::string& file);
    void update();
    void draw();

private:
    void setLevelMesh(const std::string& map, const std::string& net);

private:
    std::string m_name;

    RenderScene m_renderScene;
    RenderMesh* m_mapMesh = nullptr;

    phys::PhysicsSystem m_physSys;

    RenderLight* m_light = nullptr;
    RenderFlare* m_flare = nullptr;

    RenderSkinnedMesh*        m_rawskin = nullptr;
    anim::Animation m_animation;
    const anim::Skeleton* m_skeleton = nullptr;
    anim::Pose m_pose;
    std::vector<mat4> m_matrixPalette;
    std::vector<mat4> m_jointTransforms;
    float along = 0.f;

    Camera m_camera;
};
