#include "Lua.hpp"

namespace Lua
{
    void checkInitialization()
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base);
        lua.script(R"(print("[LUA] Lua Api Initialized Successfully!");)");
    }
}
