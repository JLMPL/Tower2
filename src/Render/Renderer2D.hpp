#pragma once
#include "Texture.hpp"
#include "Shader.hpp"
#include "Core/Math3D.hpp"
#include <vector>

namespace gfx
{

class Rectangle;
class Text;
class Label;
class Sprite;
class FadedRect;

class Renderer2D
{
public:
    Renderer2D() = default;

    void init();

    void beginFrame();
    void draw(const Rectangle& data);
    void draw(const Text& data);
    void draw(const Sprite& data);
    void draw(const FadedRect& data);
    void endFrame();

private:
    struct Shaders
    {
        Shader rectangle;
        Shader text;
        Shader sprite;
        Shader fadedRect;
    };

    Shaders m_shaders;
    mat4 m_proj;
};

extern Renderer2D g_Renderer2D;

}
