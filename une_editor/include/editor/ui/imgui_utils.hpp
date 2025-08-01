#pragma once
#include <string>
#include <imgui.h>

#include <engine/math/color.hpp>


namespace ImGui
{

void ResizableInputText(const char* label, std::string& str, bool needsEnter);
void InlinedLabel(const char* text);
void ColorEditEx(const char* label, Color &color, ImGuiColorEditFlags flags = 0);

}