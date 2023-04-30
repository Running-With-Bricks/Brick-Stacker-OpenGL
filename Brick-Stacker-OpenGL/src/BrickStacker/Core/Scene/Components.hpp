#pragma once
#include "pch.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.hpp"

namespace BrickStacker
{

	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& Name)
			: Name(Name) {}
	};

	struct TransformComponent
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position)
			: Position(position) {}

		glm::mat4 GetTransform() const
		{
			const float x{ glm::radians(Rotation.x) }, y{ glm::radians(Rotation.y) }, z{ glm::radians(Rotation.z) };
			const float c3 = glm::cos(z);
			const float s3 = glm::sin(z);
			const float c2 = glm::cos(x);
			const float s2 = glm::sin(x);
			const float c1 = glm::cos(y);
			const float s1 = glm::sin(y);
			glm::mat4 brickMatrix = glm::mat4{
			{
				Scale.x * (c1 * c3 + s1 * s2 * s3),
				Scale.x * (c2 * s3),
				Scale.x * (c1 * s2 * s3 - c3 * s1),
				0.0f,
			},
			{
				Scale.y * (c3 * s1 * s2 - c1 * s3),
				Scale.y * (c2 * c3),
				Scale.y * (c1 * c3 * s2 + s1 * s3),
				0.0f,
			},
			{
				Scale.z * (c2 * s1),
				Scale.z * (-s2),
				Scale.z * (c1 * c2),
				0.0f,
			},
			{Position.x, Position.y, Position.z, 1.0f} };
		}
	};

	struct CameraComponent
	{
		Camera Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents = ComponentGroup<NameComponent, TransformComponent, CameraComponent>;
}