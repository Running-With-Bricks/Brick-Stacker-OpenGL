#include "pch.hpp"

#include "imgui_uint_widgets.hpp"

namespace ImGui
{
	// NB: v_speed is float to allow adjusting the drag speed with more precision
	bool ImGui::DragUint(const char* label, uint32_t* v, float v_speed, uint32_t v_min, uint32_t v_max, const char* format, ImGuiSliderFlags flags)
	{
		return DragScalar(label, ImGuiDataType_U32, v, v_speed, &v_min, &v_max, format, flags);
	}
}