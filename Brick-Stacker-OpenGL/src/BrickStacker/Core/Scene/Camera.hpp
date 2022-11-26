#pragma once

#include "glm/glm.hpp"

namespace BrickStacker
{
	class Camera
	{
	public:
		Camera() {};
		~Camera() {};

		void SetOrthographicProjection(float left, float right, float top, float bottom, float Plane, float farPlane);

		void SetPerspectiveProjection(float fovy, float aspect, float nearPlane, float farPlane);

		const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

		void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0, -1, 0));
		void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0, -1, 0));
		void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

	private:
		glm::mat4 m_ProjectionMatrix{ 1 };
		glm::mat4 m_ViewMatrix{ 1 };

	};
}
