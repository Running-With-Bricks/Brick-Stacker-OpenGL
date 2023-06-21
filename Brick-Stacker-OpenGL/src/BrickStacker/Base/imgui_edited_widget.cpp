#include "pch.hpp"
#include "imgui_edited_widget.hpp"

namespace ImGui
{
	void EntityEdited(BrickStacker::Entity entity)
	{
		if (ImGui::IsItemEdited())
		{
			entity.GetComponent<BrickStacker::PhysicsComponent>().Update();
		}
	}
}