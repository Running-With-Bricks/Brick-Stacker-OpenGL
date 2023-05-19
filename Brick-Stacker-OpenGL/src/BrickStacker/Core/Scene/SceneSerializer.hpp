#pragma once
#include "pch.hpp"
#include "Scene.hpp"

namespace BrickStacker
{
	class SceneSerializer
	{
	public:
		static void Serialize(const std::string& FilePath);
		static void Deserialize(const std::string& FilePath, Ref<Scene> ActiveScene);
	};
}
