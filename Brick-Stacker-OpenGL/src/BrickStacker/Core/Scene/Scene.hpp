#pragma once
#include "pch.hpp"

#include "Components.hpp"

#include "EnTT/entt.hpp"

namespace BrickStacker
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
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

	private:
		entt::registry m_Registry;
		entt::entity m_PrimaryCamera;

		friend class Entity;
	};
}