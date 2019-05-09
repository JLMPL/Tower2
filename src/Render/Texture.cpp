#include "Texture.hpp"
#include "Image.hpp"
#include "Debug/Log.hpp"

namespace gfx
{

Texture::Texture(const char* path)
{
    loadFromFile(path);
}

Texture::~Texture()
{
    if(m_texture)
        GL(glDeleteTextures(1, &m_texture));
}

void Texture::loadFromFile(const char* path)
{
    Image img;
    img.loadFromFile(path);

    if (!img.isValid())
    {
        Log::warning("Could not load texture %s!\n", path);
        img.loadFromFile("Textures/error.png");
    }

    m_width = img.getWidth();
    m_height = img.getHeight();

    u32 format = (img.getNumChannels() == 4) ? GL_RGBA : GL_RGB;

    GL(glGenTextures(1, &m_texture));
    GL(glBindTexture(GL_TEXTURE_2D, m_texture));

    GL(glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, img.getData()));

    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

    GL(glGenerateMipmap(GL_TEXTURE_2D));
    GL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::create(u32 width, u32 height, void* data)
{
    reset();

    m_width = width;
    m_height = height;

    GL(glGenTextures(1, &m_texture));
    GL(glBindTexture(GL_TEXTURE_2D, m_texture));

    GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, data));

    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    GL(glBindTexture(GL_TEXTURE_2D, 0));
}

u32 Texture::getWidth() const
{
    return m_width;
}

u32 Texture::getHeight() const
{
    return m_height;
}

void Texture::bind() const
{
    GL(glBindTexture(GL_TEXTURE_2D, m_texture));
}

void Texture::unbind() const
{
    GL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::reset()
{
    if(m_texture)
        GL(glDeleteTextures(1, &m_texture));

    m_texture = 0;
    m_width = 0;
    m_height = 0;
}

}
