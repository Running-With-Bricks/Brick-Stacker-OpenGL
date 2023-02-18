#pragma once

#include "pch.hpp"

#include "glm/glm.hpp"

namespace BrickStacker
{
	enum class CameraType
	{
		Perspective = 0,
		Orthographic
	};

	enum class CameraBehaviour
	{
		None = 0,
		Free,
		Orbit
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
		Camera() {};
		~Camera() {};

		void SetOrthographicProjection(FrustumPlanes planes);
		void SetOrthographicProjection(float leftPlane, float rightPlane, float topPlane, float bottomPlane, float nearPlane, float farPlane);

		void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

		const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

		void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0, -1, 0));
		void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0, -1, 0));
		void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

		void Update();

		CameraType Type{ CameraType::Perspective };
		CameraBehaviour Behaviour{ CameraBehaviour::Free };

		float Aspect{ 1 };
		float FOV{ 70 };
		glm::vec3 Position{};
		glm::vec3 Rotation{};
		glm::vec3 TargetPos{};
		float Zoom = 1;
		FrustumPlanes Planes{};

	private:
		glm::mat4 m_ProjectionMatrix{ 1 };
		glm::mat4 m_ViewMatrix{ 1 };

	};
}
