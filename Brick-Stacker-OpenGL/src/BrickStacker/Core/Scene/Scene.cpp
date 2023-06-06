#include "pch.hpp"
#include "Scene.hpp"
#include "Entity.hpp"

namespace BrickStacker
{
	Scene::Scene(const std::string& filePath)
	{
		FilePath = filePath;
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

	void Scene::PushBackInstancedData(std::vector<float>& instancedData, Entity brick)
	{
		auto& transform = brick.GetComponent<TransformComponent>();
		auto& color = brick.GetComponent<ColorComponent>().Color;

		glm::mat4 brickMatrix = transform.GetTransform();

		for (size_t x = 0; x < 4; x++)
		{
			instancedData.push_back(color[x]);
		}
		instancedData.push_back(transform.Scale.x);
		instancedData.push_back(transform.Scale.z);
		for (size_t x = 0; x < 4; x++)
		{
			for (size_t y = 0; y < 3; y++)
			{
				instancedData.push_back(brickMatrix[x][y]);
			}
		}
	}

	void Scene::RenderScene()
	{
		m_Renderer.SetCamera(GetPrimaryCameraComponent().GetProjectionMatrix() * GetPrimaryCameraComponent().GetViewMatrix());

		if (BaseplateComponent& baseplateComponent = Entity(GetAllEntitiesWith<BaseplateComponent>().back(), this).GetComponent<BaseplateComponent>(); baseplateComponent.Visible)
		{
			std::vector<float> instancedData;

			for (size_t x = 0; x < 3; x++)
			{
				instancedData.push_back(baseplateComponent.Color[x]);
			}
			instancedData.push_back((float)baseplateComponent.Size);

			m_AssetManager.GetBaseplateModel()->GetVertexBuffers()[1]->UpdateBuffer(instancedData, GL_STREAM_DRAW);
			m_Renderer.Submit(m_AssetManager.GetBaseplateModel(), m_AssetManager.GetBaseplateShader(), 1);
		}

		auto bricks = GetAllEntitiesWith<BrickComponent>();

		{
			std::vector<float> instancedData;
			std::set<std::pair<float, entt::entity>, std::greater<>> transparentBricks;
			int count = 0;

			for (auto BrickEntityID : bricks)
			{
				Entity BrickEntity{ BrickEntityID, this };
				if (!BrickEntity.GetComponent<ColorComponent>().Visible)
					continue;

				count++;
				//pushback brick data into instancedData
				if (BrickEntity.GetComponent<ColorComponent>().Color.a == 1)
					PushBackInstancedData(instancedData, BrickEntity);
				else
					transparentBricks.insert({ glm::length(GetPrimaryCameraComponent().Position - BrickEntity.GetComponent<TransformComponent>().Position), BrickEntityID });
			}

			for (const auto& pair : transparentBricks)
			{
				PushBackInstancedData(instancedData, { pair.second, this });
			}

			m_AssetManager.GetBrickModel()->GetVertexBuffers()[1]->UpdateBuffer(instancedData, GL_STREAM_DRAW);

			m_Renderer.Submit(m_AssetManager.GetBrickModel(), m_AssetManager.GetBrickShader(), count);
		}
	}
}