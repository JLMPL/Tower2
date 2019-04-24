#pragma once
#include "../Geometry/ScreenQuad.hpp"
#include "../Framebuffer.hpp"
#include "../Shader.hpp"
#include "Blur.hpp"

namespace gfx
{

class Bloom
{
public:
    Bloom() = default;

    void init();

    GLuint process(GLuint texture);

private:
    void lumaPass();
    void addPass();

private:
    struct Pass
    {
        Framebuffer fbo;
        Shader      shader;
        GLuint      texture;
    };

    PostProcessBlur m_blur;

    ScreenQuad    m_screenQuad;

    GLuint        m_original;
    GLuint        m_working;

    Pass          m_luma;
    Pass          m_add;
};

}
