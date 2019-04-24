#include "Renderer2D.hpp"
#include "2D/Text.hpp"
#include "2D/Rectangle.hpp"
#include "2D/Sprite.hpp"
#include "2D/FadedRect.hpp"
#include "2D/Font.hpp"
#include "Core/Config.hpp"

namespace gfx
{

Renderer2D g_Renderer2D;

void Renderer2D::init()
{
    m_shaders.rectangle.loadFromFile("Shaders/Interface/Rectangle.sha");
    m_shaders.text.loadFromFile("Shaders/Interface/Text.sha");
    m_shaders.sprite.loadFromFile("Shaders/Interface/Sprite.sha");
    m_shaders.fadedRect.loadFromFile("Shaders/Interface/FadedRect.sha");

    auto display = core::g_Config.getDisplay();
    m_proj = math::ortho(0.f, f32(display.width), f32(display.height), 0.f);
}

void Renderer2D::beginFrame()
{
    GL(glDisable(GL_DEPTH_TEST));
    GL(glDisable(GL_CULL_FACE));
    GL(glEnable(GL_BLEND));
    GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer2D::draw(const Rectangle& rect)
{
    m_shaders.rectangle.bind();
    m_shaders.rectangle.setUniformMatrix("uProj", m_proj);
    m_shaders.rectangle.setUniform2f("uPos", rect.m_pos);
    m_shaders.rectangle.setUniformColor4("color", rect.m_color);
    m_shaders.rectangle.setUniform2f("uSize", rect.m_size);

    rect.render();

    m_shaders.rectangle.unbind();
}

void Renderer2D::draw(const Text& text)
{
    m_shaders.text.bind();
    m_shaders.text.setUniformMatrix("uProj", m_proj);

    m_shaders.text.setUniformTexture("uImage", 0, text.getTexture());

    m_shaders.text.setUniform2f("uPos", text.getPosition());
    m_shaders.text.setUniform3f("uColor",
    {
        text.m_color.x,
        text.m_color.y,
        text.m_color.z
    });

    text.render();

    m_shaders.text.unbind();
}

void Renderer2D::draw(const Sprite& sprite)
{
    m_shaders.sprite.bind();
    m_shaders.sprite.setUniformMatrix("uProj", m_proj);
    m_shaders.sprite.setUniformTexture("uImage", 0, *sprite.m_texture);
    m_shaders.sprite.setUniform2f("uPos", sprite.m_pos);
    m_shaders.sprite.setUniformColor4("uColor", sprite.m_color);

    sprite.render();

    m_shaders.sprite.unbind();
}

void Renderer2D::draw(const FadedRect& faded)
{
    m_shaders.fadedRect.bind();
    m_shaders.fadedRect.setUniformMatrix("uProj", m_proj);
    m_shaders.fadedRect.setUniform2f("uPos", faded.getPosition());
    m_shaders.fadedRect.setUniformColor4("color", faded.getColor());

    faded.render();

    m_shaders.fadedRect.unbind();
}

void Renderer2D::endFrame()
{
    GL(glDisable(GL_BLEND));
    GL(glEnable(GL_CULL_FACE));
    GL(glEnable(GL_DEPTH_TEST));
}

}
