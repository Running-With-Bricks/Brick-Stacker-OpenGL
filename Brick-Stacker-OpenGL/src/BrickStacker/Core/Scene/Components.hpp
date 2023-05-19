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
		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			: Position(position), Rotation(rotation), Scale(scale) {}

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

			return brickMatrix;
		}
	};

	struct ColorComponent
	{
		glm::vec4 Color{ 1 };
		bool Visible = true;

		ColorComponent() = default;
		ColorComponent(const ColorComponent&) = default;
		ColorComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		Camera camera{};

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		operator Camera& () { return camera; }
		operator const Camera& () const { return camera; }
	};

	enum Shape
	{
		Custom = 0,
		Cube = 1
	};
	struct BrickComponent
	{
		Shape Model = Shape::Cube;
		uint32_t ModelID = 0;

		BrickComponent() = default;
		BrickComponent(const BrickComponent&) = default;
		BrickComponent(const Shape& model)
			: Model(model) {}
	};

	struct BaseplateComponent
	{
		uint32_t Size = 100;

		BaseplateComponent() = default;
		BaseplateComponent(const BaseplateComponent&) = default;
		BaseplateComponent(const uint32_t size)
			: Size(size) {}
	};

	struct LightingComponent
	{
		glm::vec3 AmbientColor{ 0, 0, 0 };
		glm::vec3 SkyColor{ 0.49f, 0.70f, 0.90f };
		uint32_t SunIntensity = 400;

		LightingComponent() = default;
		LightingComponent(const LightingComponent&) = default;
	};

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents = ComponentGroup<NameComponent, TransformComponent, CameraComponent>;
}