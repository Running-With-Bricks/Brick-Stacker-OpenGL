#pragma once
#include "pch.hpp"
#include "Scene.hpp"
#include "Entity.hpp"

namespace BrickStacker
{
	struct SceneSerializerSettings
	{
		std::string FilePath;
		Ref<Scene> ActiveScene;
		bool ProcessBrickStackerSpecifics = true;

		SceneSerializerSettings(const std::string& filePath, Ref<Scene> activeScene, bool processBrickStackerSpecifics = true)
			: FilePath{ filePath }, ActiveScene{ activeScene }, ProcessBrickStackerSpecifics{ processBrickStackerSpecifics } {}
		~SceneSerializerSettings() = default;
	};

	class SceneSerializer
	{
	public:
		SceneSerializer(const SceneSerializer&) = delete;
		SceneSerializer& operator=(const SceneSerializer&) = delete;

		static void Serialize(SceneSerializerSettings settings);
		static void Deserialize(SceneSerializerSettings settings);
		static Ref<Scene> GetDefaultScene();
	private:
		SceneSerializer() = delete;
		~SceneSerializer() = delete;
	};
}
