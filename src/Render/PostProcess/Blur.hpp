#pragma once
#include "../Geometry/ScreenQuad.hpp"
#include "../Framebuffer.hpp"
#include "../Shader.hpp"

namespace gfx
{

class PostProcessBlur
{
public:
    enum class KernelSize
    {
        Big,
        Small
    };

    PostProcessBlur() = default;

    void init(f32 factor, KernelSize kernelSize = KernelSize::Big);

    GLuint process(GLuint texture);

private:
    void horizontalPass();
    void verticalPass();

private:
    struct Pass
    {
        Framebuffer fbo;
        Shader      shader;
        GLuint      texture;
    };

    KernelSize    m_kernelSize;

    ScreenQuad    m_screenQuad;

    GLuint        m_original;

    Pass          m_horizontal;
    Pass          m_vertical;
};

}
