#include "Config.hpp"
#include "Debug/Log.hpp"
#include "Core/Config.hpp"
#include "Core/Json.hpp"
#include <fstream>

namespace core
{

Display l_display;

void loadConfigurationFile()
{
    std::ifstream file("config.cfg");

    if (!file.good())
    {
        Log::error("config.cfg could not be loaded!\n");
        exit(EXIT_FAILURE);
    }

    json configs;
    file >> configs;

    l_display.width = configs["DisplayWidth"].get<i32>();
    l_display.halfWidth = l_display.width * 0.5;
    l_display.height = configs["DisplayHeight"].get<i32>();
    l_display.halfHeight = l_display.height * 0.5;
    l_display.fullscreen = configs["DisplayFullscreen"].get<bool>();
}

const Display& getDisplayConfig()
{
    return l_display;
}

}
