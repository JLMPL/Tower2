#pragma once
#include "Resource/AnimationCache.hpp"
#include "Render/Scene/RenderScene.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Camera.hpp"

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

    std::vector<AnimationHandle> m_animations;
    int m_currentAnim = 0;
    int m_nextAnim = 0;

    float m_speed = 1.f;

    const anim::Skeleton* m_skeleton = nullptr;
    anim::Pose m_pose;
    std::vector<mat4> m_matrixPalette;
    std::vector<mat4> m_jointTransforms;
    float along = 0.f;

    vec3 m_eye = vec3(3);

    Camera m_camera;
};
