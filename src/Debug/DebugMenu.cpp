#include "DebugMenu.hpp"
#include <ThirdParty/ImGui/imgui.h>

namespace debug
{

DebugMenu g_Menu;

DebugMenu::DebugMenu()
{
    m_entry.name = "Main";
}

void DebugMenuEntry::drawGui()
{
    if (ImGui::TreeNode(name.c_str()))
    {
        for (auto& val : values)
        {
            switch (val.second.type)
            {
                case DebugMenuEntry::Type::Bool:
                    ImGui::Checkbox(val.first.c_str(), val.second.bvalue);
                    break;
                case DebugMenuEntry::Type::Integer:
                    ImGui::InputInt(val.first.c_str(), val.second.ivalue);
                    break;
                case DebugMenuEntry::Type::Float:
                    ImGui::InputFloat(val.first.c_str(), val.second.fvalue);
                    break;
                case DebugMenuEntry::Type::CString:
                    ImGui::InputText(val.first.c_str(), val.second.svalue, 128);
                    break;
                case DebugMenuEntry::Type::Vector3:
                    ImGui::InputFloat(std::string(val.first + ".x").c_str(), &val.second.v3value->x);
                    ImGui::InputFloat(std::string(val.first + ".y").c_str(), &val.second.v3value->y);
                    ImGui::InputFloat(std::string(val.first + ".z").c_str(), &val.second.v3value->z);
                    break;
                default: break;
            }
        }

        for (auto& i : entries)
        {
            i.second.drawGui();
        }
        ImGui::TreePop();
    }
}

DebugMenuEntry& DebugMenu::operator[](const std::string& str)
{
    m_entry[str].name = str;
    return m_entry[str];
}

void DebugMenu::drawGui()
{
    if (!m_visible)
        return;

    // ImGui::ShowDemoWindow();
    ImGui::Begin("DebugMenu");
    ImGui::Text("Dis is som text");

    for (auto& i : m_entry.entries)
    {
        i.second.drawGui();
    }

    ImGui::End();
}

void DebugMenu::toggle()
{
    m_visible = !m_visible;
}

}
