#pragma once
#include "pch.hpp"
#include "BrickStacker/Core/Renderer/Renderer.hpp"
#include "BrickStacker/Base/AssetManager.hpp"
#include "Components.hpp"

#include "EnTT/entt.hpp"

namespace BrickStacker
{
	class Entity;

	class Scene
	{
	public:
		Scene(const std::string& filePath = "");
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateBrick(const std::string& name = std::string());
		Entity CreateCamera(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		Entity DuplicateEntity(Entity entity);

		Entity FindEntityByName(std::string_view name);

		void SetPrimaryCameraEntity(Entity entity);
		Entity GetPrimaryCameraEntity();
		Camera& GetPrimaryCameraComponent();

		void OnUpdate(float deltaTime);
		void RenderScene();

		template<typename Component>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Component>();
		}

		std::vector<entt::entity>& GetRoot();

		std::string FilePath = "";

	private:
		void PushBackInstancedData(std::vector<float>& instancedData, Entity brick);

		entt::registry m_Registry;
		entt::entity m_PrimaryCamera;
		entt::entity m_RootEntity;
		Renderer& m_Renderer = Renderer::Get();
		AssetManager& m_AssetManager = AssetManager::Get();

		friend class Entity;
	};
}