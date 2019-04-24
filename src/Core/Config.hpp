#pragma once
#include "Core/Types.hpp"

namespace core
{

class Config
{
public:
    struct Display
    {
        i32 width = 0;
        i32 height = 0;
        i32 halfWidth = 0;
        i32 halfHeight = 0;
        bool fullscreen = 0;
    };

    Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    void load();
    void save();

    const Display& getDisplay() const;

private:
    Display m_display;
};

extern Config g_Config;

}
