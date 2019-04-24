#include "Console.hpp"
#include "Core/Config.hpp"
#include <ImGui/imgui.h>

void Console::init()
{
}

void Console::drawGui()
{
    if (!m_visible)
        return;

    ImGui::Begin("Debug Console");
    ImGui::BeginChild("Stuff", {512, 128});

    for (u32 i = 0; i < 30; i++)
        ImGui::TextDisabled("Stuff shit");

    ImGui::EndChild();

    ImGui::InputText("command", m_buffer, 128);
    ImGui::SameLine();

    if (ImGui::Button("Execute"))
    {
        memset(m_buffer, 0, 128);
    }

    ImGui::End();
}

void Console::toggle()
{
    m_visible = false;
}
