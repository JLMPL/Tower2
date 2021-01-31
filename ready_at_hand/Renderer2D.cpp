#include "Renderer2D.hpp"
#include "2D/Text.hpp"
#include "2D/Rectangle.hpp"
#include "2D/Sprite.hpp"
#include "2D/FadedRect.hpp"
#include "2D/Font.hpp"
#include "Core/Config.hpp"
#include "Interface/HeadsUpDisplay.hpp"

namespace gfx
{

Renderer2D g_Renderer2D;

void Renderer2D::init()
{
    m_shaders.rectangle.loadFromFile("Shaders/Interface/Rectangle.sha");
    m_shaders.text.loadFromFile("Shaders/Interface/Text.sha");
    m_shaders.sprite.loadFromFile("Shaders/Interface/Sprite.sha");
    m_shaders.fadedRect.loadFromFile("Shaders/Interface/FadedRect.sha");

    auto display = core::getDisplayConfig();
    m_proj = math::ortho(0.f, f32(display.width), f32(display.height), 0.f);
}

void Renderer2D::beginFrame()
{
    GL(glDisable(GL_DEPTH_TEST));
    GL(glDisable(GL_CULL_FACE));
    GL(glEnable(GL_BLEND));
    GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer2D::draw(Rectangle& rect)
{
    m_shaders.rectangle.bind();
    m_shaders.rectangle.setUniformMatrix("uProj", m_proj);
    m_shaders.rectangle.setUniform2f("uPos", rect.m_pos);
    m_shaders.rectangle.setUniformColor4("color", rect.m_color);
    m_shaders.rectangle.setUniform2f("uSize", rect.m_size);

    rect.render();

    m_shaders.rectangle.unbind();
}

void Renderer2D::draw(Text& text)
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

void Renderer2D::draw(Sprite& sprite)
{
    m_shaders.sprite.bind();
    m_shaders.sprite.setUniformMatrix("uProj", m_proj);
    m_shaders.sprite.setUniformTexture("uImage", 0, *sprite.m_texture);
    m_shaders.sprite.setUniform2f("uPos", sprite.m_pos);
    m_shaders.sprite.setUniformColor4("uColor", sprite.m_color);

    sprite.render();

    m_shaders.sprite.unbind();
}

void Renderer2D::draw(FadedRect& faded)
{
    m_shaders.fadedRect.bind();
    m_shaders.fadedRect.setUniformMatrix("uProj", m_proj);
    m_shaders.fadedRect.setUniform2f("uPos", faded.getPosition());
    m_shaders.fadedRect.setUniformColor4("color", faded.getColor());

    faded.render();

    m_shaders.fadedRect.unbind();
}

void Renderer2D::draw(ui::HeadsUpDisplay& hud)
{
    auto display = core::getDisplayConfig();

    hud.m_hudShader.bind();
    hud.m_hudShader.setUniformMatrix("uProj", m_proj);
    hud.m_hudShader.setUniformTexture("uCircularMask", 0, hud.m_circularMask);
    hud.m_hudShader.setUniformTexture("uHealthMask", 1, hud.m_healthMask);
    hud.m_hudShader.setUniform1f("uHealth", hud.m_healthPerc);
    hud.m_hudShader.setUniform2f("uPos", vec2(32,display.height-160));
    hud.m_hudShader.setUniform3f("uColor", vec3(1));

    bindVertexArray(hud.m_hpVao);
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    unbindVertexArray(hud.m_hpVao);
    hud.m_hudShader.unbind();

    hud.m_hudShader.bind();
    hud.m_hudShader.setUniformMatrix("uProj", m_proj);
    hud.m_hudShader.setUniformTexture("uCircularMask", 0, hud.m_circularMask);
    hud.m_hudShader.setUniformTexture("uHealthMask", 1, hud.m_manaMask);
    hud.m_hudShader.setUniform1f("uHealth", hud.m_manaPerc);
    hud.m_hudShader.setUniform2f("uPos", vec2(32,display.height-160));
    hud.m_hudShader.setUniform3f("uColor", vec3(0.4));

    bindVertexArray(hud.m_hpVao);
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
    unbindVertexArray(hud.m_hpVao);
    hud.m_hudShader.unbind();
}

void Renderer2D::endFrame()
{
    GL(glDisable(GL_BLEND));
    GL(glEnable(GL_CULL_FACE));
    GL(glEnable(GL_DEPTH_TEST));
}

}
