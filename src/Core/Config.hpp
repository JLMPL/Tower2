#pragma once
#include "Core/Types.hpp"

namespace core
{

struct Display
{
    i32 width = 0;
    i32 height = 0;
    i32 halfWidth = 0;
    i32 halfHeight = 0;
    bool fullscreen = false;
};

void loadConfigurationFile();
const Display& getDisplayConfig();

}
