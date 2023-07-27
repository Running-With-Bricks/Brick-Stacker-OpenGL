#include "pch.hpp"
#include "PhysicsWorld.hpp"

#include "BrickStacker/Core/Scene/Entity.hpp"

#include "glm/ext/matrix_transform.hpp"

namespace BrickStacker
{
	PhysicsWorld::PhysicsWorld()
	{
		static auto JoltInitiated = false;

		if (!JoltInitiated)
		{
			JPH::RegisterDefaultAllocator();

			JPH::Factory::sInstance = new JPH::Factory();

			JPH::RegisterTypes();
			JPH::TempAllocatorImpl temp_allocator(1024 * 1024); //1MB, idk this probably goes unsued;
			JPH::JobSystemThreadPool job_system(1, JPH::cMaxPhysicsBarriers, 1);

			JoltInitiated = true;
		}

		// Now we can create the actual physics system.
		m_PhysicsSystem = CreateScope<JPH::PhysicsSystem>();
		m_PhysicsSystem->Init(65536, 0, 4, 4, broad_phase_layer_interface, {}, {});
	}

	PhysicsWorld::~PhysicsWorld()
	{
		
	}

	JPH::BodyID PhysicsWorld::AddBrick(Entity brick)
	{
		auto& BodyInterface = m_PhysicsSystem->GetBodyInterface();

		JPH::Vec3 pos;

		JPH::BodyCreationSettings settings;

		if (brick.HasComponent<BrickComponent>())
		{
			auto& transform = brick.GetComponent<TransformComponent>();
			pos = { transform.Position.x, transform.Position.y, transform.Position.z };
			JPH::Vec3 scale = { transform.Scale.x / 2, transform.Scale.y / 2, transform.Scale.z / 2 }; scale = scale.Abs();

			settings = { new JPH::BoxShape(scale), pos, JPH::Quat::sEulerAngles({0, glm::radians((float)transform.Rotation), 0}), JPH::EMotionType::Static, Layers::NON_MOVING };
		}
		else if (brick.HasComponent<BaseplateComponent>())
		{
			auto size = brick.GetComponent<BaseplateComponent>().Size;
			pos = { 0, 0, 0 };

			JPH::TriangleList tris{};
			tris.push_back(JPH::Triangle(JPH::Vec3{ -0.5f, 0.0f, 0.5f }*size, JPH::Vec3{ 0.5f, 0.0f, -0.5f }*size, JPH::Vec3{ 0.5f, 0.0f, 0.5f }*size));
			tris.push_back(JPH::Triangle(JPH::Vec3{ 0.5f, 0.0f, -0.5f }*size, JPH::Vec3{ -0.5f, 0.0f, 0.5f }*size, JPH::Vec3{ -0.5f, 0.0f, -0.5f }*size));

			settings = { new JPH::MeshShape(JPH::MeshShapeSettings(tris), JPH::Shape::ShapeResult()), pos, JPH::Quat::sEulerAngles({0, 0, 0}), JPH::EMotionType::Static, Layers::NON_MOVING };
		}

		Entity* copy = new Entity(brick);
		
		settings.mUserData = reinterpret_cast<JPH::uint64>(copy);
		return BodyInterface.CreateAndAddBody(settings, JPH::EActivation::DontActivate);
	}

	void PhysicsWorld::UpdateBrick(JPH::BodyID ID)
	{
		auto& BodyInterface = m_PhysicsSystem->GetBodyInterface();

		auto copy = reinterpret_cast<Entity*>(BodyInterface.GetUserData(ID));

		JPH::Vec3 pos{ 0, 0, 0 };
		JPH::Vec3 scale;
		float rot = 0;

		if (copy->HasComponent<BrickComponent>())
		{
			auto& transform = copy->GetComponent<TransformComponent>();
			pos = { transform.Position.x, transform.Position.y, transform.Position.z };
			scale = { transform.Scale.x / 2, transform.Scale.y / 2, transform.Scale.z / 2 }; scale = scale.Abs();
			rot = glm::radians((float)transform.Rotation);
			BodyInterface.SetShape(ID, new JPH::BoxShape(scale, 0.f), false, JPH::EActivation::DontActivate);
		}
		else if (copy->HasComponent<BaseplateComponent>())
		{
			auto size = copy->GetComponent<BaseplateComponent>().Size;
			pos = { 0, 0, 0 };
			rot = 0;

			JPH::TriangleList tris{};
			tris.push_back(JPH::Triangle(JPH::Vec3{ -0.5f, 0.0f, 0.5f }*size, JPH::Vec3{ 0.5f, 0.0f, -0.5f }*size, JPH::Vec3{ 0.5f, 0.0f, 0.5f }*size));
			tris.push_back(JPH::Triangle(JPH::Vec3{ 0.5f, 0.0f, -0.5f }*size, JPH::Vec3{ -0.5f, 0.0f, 0.5f }*size, JPH::Vec3{ -0.5f, 0.0f, -0.5f }*size));
			BodyInterface.SetShape(ID, new JPH::MeshShape(JPH::MeshShapeSettings(tris), JPH::Shape::ShapeResult()), false, JPH::EActivation::DontActivate);

		}
		BodyInterface.SetPositionAndRotation(ID, pos, JPH::Quat::sEulerAngles({ 0, rot, 0 }), JPH::EActivation::DontActivate);
	}

	void PhysicsWorld::DeleteBrick(JPH::BodyID ID)
	{
		BS_INFO("dsa");
		auto& BodyInterface = m_PhysicsSystem->GetBodyInterface();
		auto* ptr = reinterpret_cast<Entity*>(BodyInterface.GetUserData(ID));
		//delete ptr;

		//BodyInterface.RemoveBody(ID);
	}

	std::pair<glm::vec3, entt::entity> PhysicsWorld::Raycast(glm::vec3 pos, glm::vec3 dir)
	{
		JPH::Vec3 origin{ pos.x, pos.y, pos.z }, direction{ dir.x, dir.y, dir.z };
		JPH::RRayCast ray{ origin, direction };

		JPH::RayCastResult result;

		m_PhysicsSystem->OptimizeBroadPhase();
		auto success = m_PhysicsSystem->GetNarrowPhaseQuery().CastRay(ray, result, JPH::SpecifiedBroadPhaseLayerFilter(BroadPhaseLayers::NON_MOVING), JPH::SpecifiedObjectLayerFilter(Layers::NON_MOVING));
	
		auto* copy = reinterpret_cast<Entity*>(m_PhysicsSystem->GetBodyInterface().GetUserData(result.mBodyID));

		if (success)
			return std::pair<glm::vec3, entt::entity>(pos + result.mFraction * dir, (*copy));
		else
			return std::pair<glm::vec3, entt::entity>(glm::vec3(0), entt::null);
	}
}
