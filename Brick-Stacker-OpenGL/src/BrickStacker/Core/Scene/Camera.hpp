#pragma once
#include "pch.hpp"

#include "glm/glm.hpp"

#include "BrickStacker/Core/Input/Keyboard.hpp"
#include "BrickStacker/Core/Input/Mouse.hpp"

namespace BrickStacker
{
	class CameraController;

	enum class CameraType
	{
		Perspective = 0,
		Orthographic
	};

	enum class CameraBehaviour
	{
		None = 0,
		Free,
		Orbit,
		LookAt
	};

	struct FrustumPlanes
	{
		float Left{ -1 };
		float Right{ 1 };

		float Top{ -1 };
		float Bottom{ 1 };

		float Near{ .1f };
		float Far{ 1024 };

		FrustumPlanes() = default;
		FrustumPlanes(float leftPlane, float rightPlane, float topPlane, float bottomPlane, float nearPlane, float farPlane);
	};

	class Camera
	{
	public:
		static Ref<Camera> Create() { return CreateRef<Camera>(); };
		Camera();
		~Camera();

		void SetOrthographicProjection(FrustumPlanes planes);
		void SetOrthographicProjection(float leftPlane, float rightPlane, float topPlane, float bottomPlane, float nearPlane, float farPlane);

		void SetPerspectiveProjection(float fovy, float aspect, float nearPlane, float farPlane);

		const glm::mat4 GetWorldMatrix() const
		{
			const float x{ glm::radians(Rotation.x) }, y{ glm::radians(Rotation.y) }, z{ glm::radians(Rotation.z) };
			const float c3 = glm::cos(z);
			const float s3 = glm::sin(z);
			const float c2 = glm::cos(x);
			const float s2 = glm::sin(x);
			const float c1 = glm::cos(y);
			const float s1 = glm::sin(y);
			glm::mat4 matrix = glm::mat4{
			{
				(c1 * c3 + s1 * s2 * s3),
				(c2 * s3),
				(c1 * s2 * s3 - c3 * s1),
				0.0f,
			},
			{
				(c3 * s1 * s2 - c1 * s3),
				(c2 * c3),
				(c1 * c3 * s2 + s1 * s3),
				0.0f,
			},
			{
				(c2 * s1),
				(-s2),
				(c1 * c2),
				0.0f,
			},
			{Position.x, Position.y, Position.z, 1.0f} };
			return matrix;
		}
		const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

		void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0, -1, 0));
		void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0, -1, 0));
		void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);
		void SetViewRawYXZ(glm::vec3 position, glm::vec3 rotation);

		CameraType Type{ CameraType::Perspective };
		CameraBehaviour Behaviour{ CameraBehaviour::Free };

		float Aspect{ 1 };
		float FOV{ 70 };
		glm::vec3 Position{ 0, 0, 0 };
		glm::vec3 Rotation{ 0, 0, 0 };
		glm::vec3 TargetPos{ 0, 0, 0 };
		float Zoom = 1;
		FrustumPlanes Planes{};

		float Distance = 10;

	private:
		glm::mat4 m_ProjectionMatrix{ 1 };
		glm::mat4 m_ViewMatrix{ 1 };

		glm::vec3 m_OrbitRotation{ 0, 0, 0 };

		friend class CameraController;
	};
}
