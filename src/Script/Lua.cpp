#include "Lua.hpp"

namespace sol
{
    void checkInitialization()
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base);
        lua.script(R"(print("[LUA] Lua is working properly!");)");
    }
}

