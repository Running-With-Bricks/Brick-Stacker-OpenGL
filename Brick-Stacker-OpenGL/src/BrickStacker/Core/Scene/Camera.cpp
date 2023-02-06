#include "pch.hpp"
#include "Camera.hpp"

namespace BrickStacker
{
	void Camera::SetOrthographicProjection(FrustumPlanes planes)
	{
		Planes = planes;

		m_ProjectionMatrix = glm::mat4{ 1.0f };
		m_ProjectionMatrix[0][0] = 2.f / (Planes.Right - Planes.Left) * Zoom;
		m_ProjectionMatrix[1][1] = 2.f / (Planes.Bottom - Planes.Top) * Zoom;
		m_ProjectionMatrix[2][2] = 1.f / (Planes.Far - Planes.Near);
		m_ProjectionMatrix[3][0] = -(Planes.Right + Planes.Left) / (Planes.Right - Planes.Left);
		m_ProjectionMatrix[3][1] = -(Planes.Bottom + Planes.Top) / (Planes.Bottom - Planes.Top);
		m_ProjectionMatrix[3][2] = -Planes.Near / (Planes.Far - Planes.Near);
	}

	void Camera::SetOrthographicProjection(float leftPlane, float rightPlane, float topPlane, float bottomPlane, float nearPlane, float farPlane)
	{
		FrustumPlanes newPlanes{ leftPlane, rightPlane, topPlane, bottomPlane, nearPlane, farPlane };

		SetOrthographicProjection(newPlanes);
	}

	void Camera::SetPerspectiveProjection(float fov, float aspect, float nearPlane, float farPlane)
	{
		Aspect = aspect;
		FOV = fov;

		Planes.Near = nearPlane;
		Planes.Far = farPlane;

		fov = glm::radians(FOV);
		assert(glm::abs(Aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFov = tan(fov / 2.f);
		m_ProjectionMatrix = glm::mat4{ 0.0f };
		m_ProjectionMatrix[0][0] = 1.f / (Aspect * tanHalfFov);
		m_ProjectionMatrix[1][1] = 1.f / (tanHalfFov);
		m_ProjectionMatrix[2][2] = Planes.Far / (Planes.Far - Planes.Near);
		m_ProjectionMatrix[2][3] = 1.f;
		m_ProjectionMatrix[3][2] = -(Planes.Far * Planes.Near) / (Planes.Far - Planes.Near);
	}

	void Camera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
	{
		Position = position;
		TargetPos = Position + direction;

		const glm::vec3 w{ glm::normalize(direction) };
		const glm::vec3 u{ glm::normalize(glm::cross(w, up)) };
		const glm::vec3 v{ glm::cross(w, u) };

		m_ViewMatrix = glm::mat4{ 1.f };
		m_ViewMatrix[0][0] = u.x;
		m_ViewMatrix[1][0] = u.y;
		m_ViewMatrix[2][0] = u.z;
		m_ViewMatrix[0][1] = v.x;
		m_ViewMatrix[1][1] = v.y;
		m_ViewMatrix[2][1] = v.z;
		m_ViewMatrix[0][2] = w.x;
		m_ViewMatrix[1][2] = w.y;
		m_ViewMatrix[2][2] = w.z;
		m_ViewMatrix[3][0] = -glm::dot(u, Position);
		m_ViewMatrix[3][1] = -glm::dot(v, Position);
		m_ViewMatrix[3][2] = -glm::dot(w, Position);
	}

	void Camera::SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
	{
		SetViewDirection(position, target - position, up);
	}

	void Camera::SetViewYXZ(glm::vec3 position, glm::vec3 rotation)
	{
		Position = position;
		Rotation = rotation;

		const float x{ glm::radians(glm::clamp(Rotation.x, -90.f, 90.f)) }, y{ glm::radians(Rotation.y) }, z{ glm::radians(Rotation.z) };
		const float c3 = glm::cos(z);
		const float s3 = glm::sin(z);
		const float c2 = glm::cos(x);
		const float s2 = glm::sin(x);
		const float c1 = glm::cos(y);
		const float s1 = glm::sin(y);
		const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
		const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
		const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
		m_ViewMatrix = glm::mat4{ 1.f };
		m_ViewMatrix[0][0] = u.x;
		m_ViewMatrix[1][0] = u.y;
		m_ViewMatrix[2][0] = u.z;
		m_ViewMatrix[0][1] = v.x;
		m_ViewMatrix[1][1] = v.y;
		m_ViewMatrix[2][1] = v.z;
		m_ViewMatrix[0][2] = w.x;
		m_ViewMatrix[1][2] = w.y;
		m_ViewMatrix[2][2] = w.z;
		m_ViewMatrix[3][0] = -glm::dot(u, Position);
		m_ViewMatrix[3][1] = -glm::dot(v, Position);
		m_ViewMatrix[3][2] = -glm::dot(w, Position);
	}

	void Camera::Update()
	{
		if (Type == CameraType::Orthographic)
		{
			SetOrthographicProjection(Planes);
		}
		else if (Type == CameraType::Perspective)
		{
			SetPerspectiveProjection(FOV, Aspect, Planes.Near, Planes.Far);
		}

		if (Behaviour == CameraBehaviour::None)
		{

		}
		else if (Behaviour == CameraBehaviour::Free)
		{
			SetViewYXZ(Position, Rotation);
		}
		else if (Behaviour == CameraBehaviour::Orbit)
		{
			SetViewTarget(Position, TargetPos);
		}
	}

	FrustumPlanes::FrustumPlanes(float leftPlane, float rightPlane, float topPlane, float bottomPlane, float nearPlane, float farPlane)
	{
		Left = leftPlane;
		Right = rightPlane;

		Top = topPlane;
		Bottom = bottomPlane;

		Near = nearPlane;
		Far = farPlane;
	}
}
