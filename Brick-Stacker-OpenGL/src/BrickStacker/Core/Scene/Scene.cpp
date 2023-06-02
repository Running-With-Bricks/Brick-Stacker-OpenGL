#include "pch.hpp"
#include "Scene.hpp"
#include "Entity.hpp"

namespace BrickStacker
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		//m_KeyboardCallback->Disconnect();
		//m_MouseButtonCallback->Disconnect();
		//m_MousePositionCallback->Disconnect();
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<NameComponent>(name);

		return entity;
	}

	Entity Scene::CreateBrick(const std::string& name)
	{
		auto Brick = CreateEntity(!name.empty() ? name : std::string("Brick-")+std::to_string(m_Registry.size()));
		Brick.AddComponent<BrickComponent>();
		Brick.AddComponent<ColorComponent>(glm::vec4(0.5f, 0.5f, 0.5f, 1));
		Brick.AddComponent<TransformComponent>();

		return Brick;
	}

	Entity Scene::CreateCamera(const std::string& name)
	{
		auto camera = CreateEntity(!name.empty() ? name : "Camera");
		camera.AddComponent<CameraComponent>();

		return camera;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(float deltaTime)
	{
		// Render
		RenderScene();
	}

	void Scene::SetPrimaryCameraEntity(Entity entity)
	{
		m_PrimaryCamera = entity;
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		return Entity(m_PrimaryCamera, this);
	}

	Camera& Scene::GetPrimaryCameraComponent()
	{
		return GetPrimaryCameraEntity().GetComponent<CameraComponent>();
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		// Copy name because we're going to modify component data structure
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}

	Entity Scene::FindEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<NameComponent>();
		for (auto entity : view)
		{
			const NameComponent& tc = view.get<NameComponent>(entity);
			if (tc.Name == name)
				return Entity{ entity, this };
		}

		BS_ERROR("Couldn't find Entity with name \"{0}\"", name);
		return {};
	}

	void Scene::RenderScene()
	{

	}
}