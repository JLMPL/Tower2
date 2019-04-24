#pragma once
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "ArrayBuffer.hpp"
#include "Core/Math3D.hpp"
#include <vector>

namespace gfx
{

class LineRenderer
{
public:
    void init();

    void addLine(const vec3& a, const vec3& b, const vec3& c);

    void prepare();
    void render(const mat4& view, const mat4& proj);
    void clear();

private:
    struct LinePos
    {
        vec3 a, b;
    };

    struct LineColor
    {
        vec3 c0, c1;
    };

    std::vector<LinePos> m_posData;
    std::vector<LineColor> m_colorData;

    Shader m_shader;

    VertexArray m_vao;
    ArrayBuffer m_vbo;
    ArrayBuffer m_cbo;

    bool m_needsUpdate = false;
};

}
