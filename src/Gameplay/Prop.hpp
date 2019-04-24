#pragma once
#include "Entity.hpp"
#include <Render/Renderer3D.hpp>
#include <Render/Light.hpp>

namespace gfx
{
    class StaticMesh;
}

class Prop
{
public:
    void init(const gfx::StaticMesh* mesh,
        const mat4* tr = nullptr, bool hasLight = false);
    void updateTransforms();

    void setMesh(const gfx::StaticMesh* mesh);
    void setLocalTransform(const mat4& tr);
    void setParentTransform(const mat4* tr);

    void enableLight();
    void disableLight();

    void show();
    void hide();

private:
    const mat4* m_parent = nullptr;
    mat4        m_transform;
    mat4        m_localTransform;

    gfx::RenderableHandle m_mesh;
    gfx::Light            m_light;
    bool                  m_hasLight = false;
};
