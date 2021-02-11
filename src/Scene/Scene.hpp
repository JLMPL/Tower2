#pragma once
#include "Resource/AnimationCache.hpp"
#include "Render/Scene/RenderScene.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Animation/SkinningData.hpp"
#include "Camera.hpp"
#include "ThirdParty/entt/entt.hpp"

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

    entt::registry m_registry;

    RenderScene m_renderScene;
    RenderMesh* m_mapMesh = nullptr;

    phys::PhysicsSystem m_physSys;

    RenderLight* m_light = nullptr;
    RenderFlare* m_flare = nullptr;

    std::vector<AnimationHandle> m_animations;

    vec3 m_eye = vec3(3);

    Camera m_camera;
};
