#include "Config.hpp"
#include "Debug/Log.hpp"
#include "Core/Config.hpp"
#include "Core/Json.hpp"
#include <fstream>

namespace core
{

Config g_Config;

void Config::load()
{
    std::ifstream file("config.cfg");

    if (!file.good())
    {
        Log::error("config.cfg could not be loaded!\n");
        exit(EXIT_FAILURE);
    }

    json configs;
    file >> configs;

    m_display.width = configs["DisplayWidth"].get<i32>();
    m_display.halfWidth = m_display.width * 0.5;
    m_display.height = configs["DisplayHeight"].get<i32>();
    m_display.halfHeight = m_display.height * 0.5;
    m_display.fullscreen = configs["DisplayFullscreen"].get<bool>();
}

void Config::save()
{
}

const Config::Display& Config::getDisplay() const
{
    return m_display;
}

}
