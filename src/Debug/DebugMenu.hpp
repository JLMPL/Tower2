#pragma once
#include "Core/Math3D.hpp"
#include <map>

namespace debug
{

struct DebugMenuEntry
{
    enum Type
    {
        Integer,
        Float,
        CString,
        Vector3
    };

    struct Value
    {
        Type type;
        union
        {
            i32* ivalue;
            f32* fvalue;
            char* svalue;
            vec3* v3value;
        };

        Value()
        {
            ivalue = nullptr;
        }
    };

    std::map<std::string, DebugMenuEntry> entries;
    std::map<std::string, Value> values;
    std::string name;

    DebugMenuEntry& operator[](const std::string& name)
    {
        entries[name].name = name;
        return entries[name];
    }

    void bind(const std::string& n, i32* val)
    {
        values[n].ivalue = val;
        values[n].type = Type::Integer;
    }

    void bind(const std::string& n, f32* val)
    {
        values[n].fvalue = val;
        values[n].type = Type::Float;
    }

    void bind(const std::string& n, char* val)
    {
        values[n].svalue = val;
        values[n].type = Type::CString;
    }

    void bind(const std::string& n, vec3* val)
    {
        values[n].v3value = val;
        values[n].type = Type::Vector3;
    }

    void drawGui();
};

class DebugMenu
{
public:
    DebugMenu();
    ~DebugMenu() = default;

    DebugMenuEntry& operator[](const std::string& str);
    void drawGui();

    void toggle();

private:
    bool m_visible = false;
    DebugMenuEntry m_entry;
};

extern DebugMenu g_Menu;

}
