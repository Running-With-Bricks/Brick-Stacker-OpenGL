#pragma once

#include "ImGui/imgui.h"
#include "imgui_stdlib.h"
#include "ImGui/backends/imgui_impl_glfw.h"
#include "ImGui/backends/imgui_impl_opengl3.h"

namespace ImGui
{
	bool DragUint(const char* label, uint32_t* v, float v_speed, uint32_t v_min, uint32_t v_max, const char* format, ImGuiSliderFlags flags);
}
