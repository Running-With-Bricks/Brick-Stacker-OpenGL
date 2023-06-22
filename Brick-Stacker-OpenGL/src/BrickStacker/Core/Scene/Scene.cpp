#include "pch.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "BrickStacker/Base/AppSettings.hpp"

namespace BrickStacker
{
	Scene::Scene(const std::string& filePath)
	{
		FilePath = filePath;
		Entity root = { m_Registry.create(), this };
		root.AddComponent<NameComponent>("ROOT_ENTITY");
		m_RootEntity = root;
		root.AddComponent<ChildComponent>();

		m_PhysicsWorld = PhysicsWorld::Create();
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
		entity.AddComponent<ParentComponent>().parent = m_RootEntity;
		GetRoot().push_back(entity);

		return entity;
	}

	Entity Scene::CreateBrick(const std::string& name)
	{
		auto Brick = CreateEntity(!name.empty() ? name : std::string("Brick-")+std::to_string(m_Registry.size()));
		Brick.AddComponent<BrickComponent>();
		Brick.AddComponent<ColorComponent>(glm::vec4(0.5f, 0.5f, 0.5f, 1));
		Brick.AddComponent<TransformComponent>();
		Brick.AddComponent<PhysicsComponent>(m_PhysicsWorld->AddBrick(Brick), m_PhysicsWorld);

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
		auto& children = GetRoot();
		children.erase(std::remove(children.begin(), children.end(), entity), children.end());
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(float deltaTime)
	{
		// Render
		RenderScene();
	}

	std::pair<glm::vec3, entt::entity> Scene::Raycast(const Camera& camera, glm::vec2 mouseCoords)
	{
		glm::vec4 lRayStart_NDC(
			mouseCoords.x,
			-mouseCoords.y,
			-1.0f,
			1.0f
		);
		glm::vec4 lRayEnd_NDC(
			mouseCoords.x,
			-mouseCoords.y,
			0.0f,
			1.0f
		);

		glm::mat4 M = glm::inverse(camera.GetProjectionMatrix() * camera.GetViewMatrix());
		glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world /= lRayStart_world.w;
		glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world   /= lRayEnd_world.w;

		glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
		lRayDir_world = glm::normalize(lRayDir_world) * camera.Planes.Far;

		return m_PhysicsWorld->Raycast(lRayStart_world, lRayDir_world);
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

		if (!AppSettings::Get().CollisionRendering)
		{
			for (size_t x = 0; x < 4; x++)
			{
				instancedData.push_back(color[x]);
			}
		}
		else
		{
			instancedData.push_back(brick.GetComponent<BrickComponent>().Collision);
			instancedData.push_back(0);
			instancedData.push_back(!brick.GetComponent<BrickComponent>().Collision);
			instancedData.push_back(1);
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

			if (!AppSettings::Get().CollisionRendering)
			{
				for (size_t x = 0; x < 3; x++)
				{
					instancedData.push_back(baseplateComponent.Color[x]);
				}
			}
			else
			{
				instancedData.push_back(1);
				instancedData.push_back(0);
				instancedData.push_back(0);
			}
			instancedData.push_back((float)baseplateComponent.Size);

			m_AssetManager.GetBaseplateModel()->GetVertexBuffers()[1]->UpdateBuffer(instancedData, GL_STREAM_DRAW);
			m_Renderer.Submit(AppSettings::Get().WireframeBaseplateRendering ? m_AssetManager.GetWireframeBaseplateModel() : m_AssetManager.GetBaseplateModel(), m_AssetManager.GetBaseplateShader(), 1);
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

			m_Renderer.Submit(AppSettings::Get().WireframeBrickRendering ? m_AssetManager.GetWireframeBrickModel() : m_AssetManager.GetBrickModel(), m_AssetManager.GetBrickShader(), count);
		}
	}

	std::vector<entt::entity>& Scene::GetRoot()
	{
		return Entity(m_RootEntity, this).GetComponent<ChildComponent>().children;
	}
}