#pragma once
#include "pch.hpp"
#include "BrickStacker/Core/Renderer/Renderer.hpp"
#include "BrickStacker/Base/AssetManager.hpp"
#include "Components.hpp"

#include "EnTT/entt.hpp"

namespace BrickStacker
{
	class Entity;
	class SceneSerializer;

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

		std::pair<glm::vec3, entt::entity> Raycast(glm::vec3 pos, glm::vec3 dir) { m_PhysicsWorld->Raycast(pos, dir); };
		std::pair<glm::vec3, entt::entity> Raycast(const Camera& camera, glm::vec2 mouseCoords);

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
		Ref<PhysicsWorld> m_PhysicsWorld;
		Renderer& m_Renderer = Renderer::Get();
		AssetManager& m_AssetManager = AssetManager::Get();

		friend class Entity;
		friend class SceneSerializer;
	};
}