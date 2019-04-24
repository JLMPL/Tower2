#pragma once
#include "Core/Types.hpp"

namespace gfx
{

class Image
{
public:
    struct Pixel
    {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    };

    Image() = default;
    ~Image();

    void loadFromFile(CString path);

    bool isValid() const;

    u32 getWidth() const;
    u32 getHeight() const;

    u32 getNumChannels() const;
    u32 getNumPixels() const;

    Pixel getPixel(u32 x, u32 y) const;

    const u8* getData() const;

private:
    u32 m_channels = 0u;

    u32 m_width = 0u;
    u32 m_height = 0u;

    u8* m_data = nullptr;
};

}
