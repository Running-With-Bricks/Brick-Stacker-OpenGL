#pragma once

#include "imgui_includes.hpp"

namespace ImGui
{
	bool DragUint(const char* label, uint32_t* v, float v_speed, uint32_t v_min, uint32_t v_max, const char* format, ImGuiSliderFlags flags);
}
