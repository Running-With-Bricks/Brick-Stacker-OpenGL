#pragma once
#include "pch.hpp"
#include "Scene.hpp"
#include "Entity.hpp"

namespace BrickStacker
{
	class SceneSerializer
	{
	public:
		static void Serialize(const std::string& FilePath, Ref<Scene> ActiveScene);
		static void Deserialize(const std::string& FilePath, Ref<Scene> ActiveScene);
		static Ref<Scene> GetDefaultScene();
	};
}
