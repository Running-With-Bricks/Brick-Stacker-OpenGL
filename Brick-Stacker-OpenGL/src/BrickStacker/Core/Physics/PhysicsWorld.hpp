#pragma once
#include "pch.hpp"

#include "Jolt/Jolt.h"

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/CastResult.h>

#include "EnTT/entt.hpp"
#include "glm/glm.hpp"

#pragma region Boilerplate
namespace Layers
{
	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 1;
};

namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::uint NUM_LAYERS(1);
};

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
{
public:
	BPLayerInterfaceImpl()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
	}

	virtual JPH::uint					GetNumBroadPhaseLayers() const override
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	virtual JPH::BroadPhaseLayer			GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
	{
		using namespace JPH;
		JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

private:
	JPH::BroadPhaseLayer					mObjectToBroadPhase[Layers::NUM_LAYERS];
};
#pragma endregion

namespace BrickStacker
{
	class Entity;

	class PhysicsWorld
	{
	public:
		static Ref<PhysicsWorld> Create() { return CreateRef<PhysicsWorld>(); }
		PhysicsWorld();
		~PhysicsWorld();

		JPH::BodyID AddBrick(Entity brick);
		void UpdateBrick(JPH::BodyID brick);
		void DeleteBrick(JPH::BodyID brick);

		std::pair<glm::vec3, entt::entity> Raycast(glm::vec3 pos, glm::vec3 dir);

	private:
		Scope<JPH::PhysicsSystem> m_PhysicsSystem;
		BPLayerInterfaceImpl broad_phase_layer_interface;

		static const JPH::uint c_MaxBodies = 65536;
		static const JPH::uint c_NumBodyMutexes = 0;
		static const JPH::uint c_MaxBodyPairs = 4;
		static const JPH::uint c_MaxContactConstraints = 4;
	};
}
