#include "pch.hpp"
#include "Camera.hpp"

#include "BrickStacker/Application/Application.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace BrickStacker
{
	Camera::Camera()
	{
		m_KeyboardCallback = CreateRef<BrickStacker::KeyCallback>(Keyboard::AddCallback([this](Keycode key, KeyAction action)
			{
				if (!ListenToKeyboardInput || !Application::Get().IsFocusedOnViewport()) { return; };

				if (Behaviour == CameraBehaviour::Free)
				{
					if (!Keyboard::IsKeyDown(Keys::W) && !Keyboard::IsKeyDown(Keys::A) && !Keyboard::IsKeyDown(Keys::S) && !Keyboard::IsKeyDown(Keys::D) && !Keyboard::IsKeyDown(Keys::Q) && !Keyboard::IsKeyDown(Keys::E))
					{
						m_Moving = false;
					}
					else if (!m_Moving)
					{
						m_Moving = true;
						m_MovementTime = 0;
					}
				}
			}));

		m_MouseButtonCallback = CreateRef<BrickStacker::MouseButtonCallback>(Mouse::AddMouseButtonCallback([this](MouseButton button, KeyAction action)
			{
				if (!ListenToMouseInput || !Application::Get().IsFocusedOnViewport()) { return; };

				if (Behaviour == CameraBehaviour::Free)
				{
					if (button == MouseButtons::Right && action == KeyAction::Down)
					{
						m_Rotating = true;
						Mouse::Get().UpdateImGui = false;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					}
					else if (button == MouseButtons::Right && action == KeyAction::Up)
					{
						m_Rotating = false;
						Mouse::Get().UpdateImGui = true;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
				}
				else if (Behaviour == CameraBehaviour::Orbit)
				{
					if (button == MouseButtons::Middle && action == KeyAction::Down && !Keyboard::IsKeyDown(Keys::LeftShift))
					{
						m_Rotating = true;
						Keyboard::Get().UpdateImGui = false;
						Mouse::Get().UpdateImGui = false;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					}
					else if (button == MouseButtons::Middle && action == KeyAction::Up && !m_Moving)
					{
						m_Rotating = false;
						Keyboard::Get().UpdateImGui = true;
						Mouse::Get().UpdateImGui = true;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
					else if (button == MouseButtons::Middle && action == KeyAction::Down && Keyboard::IsKeyDown(Keys::LeftShift))
					{
						m_Moving = true;
						m_PrevMousePos = Mouse::GetPosition();
						BS_INFO("++{0} | {1}", m_PrevMousePos.x, m_PrevMousePos.y);
						Keyboard::Get().UpdateImGui = false;
						Mouse::Get().UpdateImGui = false;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					}
					else if (button == MouseButtons::Middle && action == KeyAction::Up && m_Moving)
					{
						m_Moving = false;
						m_PrevMoving = m_Moving;
						Keyboard::Get().UpdateImGui = true;
						Mouse::Get().UpdateImGui = true;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
				}
			}));

		m_MousePositionCallback = CreateRef<BrickStacker::MousePositionCallback>(Mouse::AddMousePositionCallback([this](glm::vec2 pos)
			{
				if (!ListenToMouseInput || !Application::Get().IsFocusedOnViewport()) { return; };

				if (Behaviour == CameraBehaviour::Free)
				{
					if (m_Rotating && !m_PrevRotating)
					{
						m_PrevMousePos = pos;

					}
					else if (m_Rotating && m_PrevRotating)
					{
						auto currMousePos = pos;
						auto delta = m_PrevMousePos - currMousePos;
						Rotation += glm::vec3(delta.y * (InvertY - 0.5f) * 2, -delta.x * (InvertX - 0.5f) * 2, 0) * Sensitivity;
						Rotation = { glm::clamp(Rotation.x, -90.f, 90.f), Rotation.y, Rotation.z };
						m_PrevMousePos = currMousePos;
					}
				}

				if (Behaviour == CameraBehaviour::Orbit)
				{
					if (m_Rotating && !m_PrevRotating)
					{
						m_PrevMousePos = pos;

					}
					else if (m_Rotating && m_PrevRotating)
					{
						auto currMousePos = pos;
						auto delta = m_PrevMousePos - currMousePos;
						m_OrbitRotation += glm::vec3(delta.y * (InvertY - 0.5f) * 2, -delta.x * (InvertX - 0.5f) * 2, 0) * Sensitivity;
						m_PrevMousePos = currMousePos;
					}

					if (m_Moving && !m_PrevMoving)
						m_PrevMousePos = pos;

					if (m_Moving)
					{
						auto currMousePos = pos;
						auto delta = m_PrevMousePos - currMousePos;
						BS_INFO("{0} {1} | {2} {3} | {4} {5}", delta.x, delta.y, m_PrevMousePos.x, m_PrevMousePos.y, currMousePos.x, currMousePos.y);
						m_MovementVector = glm::vec3(delta.x * (InvertY - 0.5f) * 2, delta.y * (InvertX - 0.5f) * 2, 0) * 0.3f * Sensitivity;
						m_PrevMousePos = currMousePos;
					}

					m_PrevMoving = m_Moving;
				}

				m_PrevRotating = m_Rotating;
			}));

		m_MouseScrollCallback = CreateRef<BrickStacker::MouseScrollCallback>(Mouse::AddMouseScrollCallback([this](glm::vec2 pos)
			{
				if (!ListenToMouseInput || !Application::Get().IsFocusedOnViewport()) { return; };

				if (Behaviour == CameraBehaviour::Orbit)
				{
					auto newD = Distance + pos.y * (InvertScroll - 0.5f) * 2;
					Distance = (newD <= 0.0125f) ? 0.0125f : newD;
				}
			}));
	}

	Camera::~Camera()
	{
		m_KeyboardCallback->Disconnect();
		m_MouseButtonCallback->Disconnect();
		m_MousePositionCallback->Disconnect();
	}

	void Camera::SetOrthographicProjection(FrustumPlanes planes)
	{
		Planes = planes;

		m_ProjectionMatrix = glm::mat4{ 1.0f };
		m_ProjectionMatrix[0][0] = -2.0f / (Planes.Right - Planes.Left) * Zoom * 0.01f;
		m_ProjectionMatrix[1][1] = 2.0f / (Planes.Bottom - Planes.Top) * Zoom * 0.01f;
		m_ProjectionMatrix[2][2] = 1.0f / (Planes.Far - Planes.Near);
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
		BS_ASSERT(glm::abs(Aspect - std::numeric_limits<float>::epsilon()) > 0.0f, "Aspect is zero");
		const float tanHalfFov = tan(fov / 2.0f);
		m_ProjectionMatrix = glm::mat4{ 0.0f };
		m_ProjectionMatrix[0][0] = -1.0f / (Aspect * tanHalfFov);
		m_ProjectionMatrix[1][1] = 1.0f / (tanHalfFov);
		m_ProjectionMatrix[2][2] = Planes.Far / (Planes.Far - Planes.Near);
		m_ProjectionMatrix[2][3] = 1.0f;
		m_ProjectionMatrix[3][2] = -(Planes.Far * Planes.Near) / (Planes.Far - Planes.Near);
	}

	void Camera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
	{
		Position = position;
		TargetPos = Position + direction;

		if (Position.x == TargetPos.x && Position.z == TargetPos.z)
		{
			auto rot = Rotation;
			SetViewYXZ(position, glm::vec3(90 * (TargetPos.y <= Position.y ? 1 : -1), 0, 0));
			Rotation = rot;
			return;
		}

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
		Rotation = { glm::clamp(rotation.x, -90.f, 90.f), rotation.y, rotation.z };

		SetViewRawYXZ(Position, Rotation);
	}

	void Camera::SetViewRawYXZ(glm::vec3 position, glm::vec3 rotation)
	{
		const float x{ glm::radians(rotation.x) }, y{ glm::radians(rotation.y) }, z{ glm::radians(rotation.z) };
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
		m_ViewMatrix[3][0] = -glm::dot(u, position);
		m_ViewMatrix[3][1] = -glm::dot(v, position);
		m_ViewMatrix[3][2] = -glm::dot(w, position);
	}

	void Camera::Update(float deltaTime)
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
			m_MovementVector =
			{
				Keyboard::IsKeyDown(Keys::A) - Keyboard::IsKeyDown(Keys::D),
				Keyboard::IsKeyDown(Keys::E) - Keyboard::IsKeyDown(Keys::Q),
				Keyboard::IsKeyDown(Keys::W) - Keyboard::IsKeyDown(Keys::S)
			};

			auto currViewportFocused = Application::Get().IsFocusedOnViewport();
			if (currViewportFocused && !m_PrevViewportFocused)
				m_MovementTime = 0;

			if (m_Moving && currViewportFocused && m_PrevViewportFocused)
			{
				auto t = m_MovementTime - AccelerateAfter;

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
					0,
				},
				{
					(c3 * s1 * s2 - c1 * s3),
					(c2 * c3),
					(c1 * c3 * s2 + s1 * s3),
					0,
				},
				{
					(c2 * s1),
					(-s2),
					(c1 * c2),
					0,
				},
				{0, 0, 0, 0} };

				auto newMovement = glm::vec3(matrix * glm::vec4(m_MovementVector, 1));
				auto useDT = IgnoreDeltaTime ? 1 : deltaTime;
				auto shiftMultiplier = Keyboard::IsKeyDown(Keys::LeftShift) ? ShiftMultiplier : 1;
				Position += newMovement * (shiftMultiplier + (0.5f * Acceleration * t * t)) * Speed * useDT;
				m_MovementTime += deltaTime;
			}
			m_PrevViewportFocused = currViewportFocused;

			SetViewYXZ(Position, Rotation);
		}
		else if (Behaviour == CameraBehaviour::Orbit)
		{
			auto currViewportFocused = Application::Get().IsFocusedOnViewport();

			const float x{ glm::radians(m_OrbitRotation.x) }, y{ glm::radians(m_OrbitRotation.y) }, z{ glm::radians(m_OrbitRotation.z) };
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
				0,
			},
			{
				(c3 * s1 * s2 - c1 * s3),
				(c2 * c3),
				(c1 * c3 * s2 + s1 * s3),
				0,
			},
			{
				(c2 * s1),
				(-s2),
				(c1 * c2),
				0,
			},
			{0, 0, 0, 0} };

			auto newMovement = glm::vec3(matrix * glm::vec4(0, 0, -Distance, 1));
			auto newTarget = glm::vec3(matrix * glm::vec4(m_MovementVector.x, m_MovementVector.y, 0, 1));

			TargetPos = newTarget + TargetPos;
			Position = newMovement + TargetPos;
			m_MovementTime += deltaTime;

			m_MovementVector = glm::vec3(0);
			m_PrevViewportFocused = currViewportFocused;

			SetViewRawYXZ(Position, m_OrbitRotation);
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
