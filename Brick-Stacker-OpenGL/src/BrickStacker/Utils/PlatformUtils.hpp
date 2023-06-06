#pragma once

#include "pch.hpp"
#include "BrickStacker/Core/Scene/Scene.hpp"

namespace BrickStacker
{
	class FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);

		//returns whether code should proceed
		static bool ChangeFile(Ref<Scene> scene);
	};
}
