#pragma once
#include "Core/Math3D.hpp"
#include <vector>
#include <memory>

class RenderEntity
{
public:
    using Ptr = std::unique_ptr<RenderEntity>;

    enum class Type
    {
        Mesh,
        SkinMesh,
        Light,
        Flare,
        Cloth
    };

    template <typename T>
    T* as()
    {
        return static_cast<T*>(this);
    }

    RenderEntity() = default;
    ~RenderEntity() = default;

    void setTransform(const mat4& tr);
    const mat4& getTransform() const;

    void show();
    void hide();

    bool isVisible() const;

    virtual Type getType() const = 0;

protected:
    mat4 m_transform;

    bool m_visible = true;
};
