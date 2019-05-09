#include "Image.hpp"
#include "Debug/Log.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace gfx
{

Image::~Image()
{
    if (m_data)
        stbi_image_free(m_data);
}

void Image::loadFromFile(const char* path)
{
    i32 wid, hei, chan;
    m_data = stbi_load(path, &wid, &hei, &chan, 0);

    if (!m_data)
    {
        Log::warning("Could not load image %s!\n", path);
        return;
    }

    m_width = u32(wid);
    m_height = u32(hei);
    m_channels = u32(chan);
}

bool Image::isValid() const
{
    return (m_channels && m_data);
}

u32 Image::getWidth() const
{
    return m_width;
}

u32 Image::getHeight() const
{
    return m_height;
}

u32 Image::getNumChannels() const
{
    return m_channels;
}

u32 Image::getNumPixels() const
{
    return m_width * m_height;
}

Image::Pixel Image::getPixel(u32 x, u32 y) const
{
    u64 start = reinterpret_cast<u64>(m_data);
    u64 offset = (y * m_width * m_channels) + (x * m_channels);

    Pixel* px = reinterpret_cast<Pixel*>(start + offset);

    return *px;
}

const u8* Image::getData() const
{
    return m_data;
}

}
