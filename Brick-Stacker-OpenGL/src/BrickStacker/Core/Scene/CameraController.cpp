#include "pch.hpp"
#include "CameraController.hpp"

#include "BrickStacker/Application/Application.hpp"

namespace BrickStacker
{
	CameraController::CameraController()
	{
		m_KeyboardCallback = CreateRef<BrickStacker::KeyCallback>(Keyboard::AddCallback([this](Keycode key, KeyAction action)
			{
				if (!CameraSettings.ListenToKeyboardInput || !Application::Get().GetFocusedScene()) { return; };
		
				auto Camera = GetWorkingCamera();
				if (Camera == nullptr) { return; }

				if (Camera->Behaviour == CameraBehaviour::Free)
				{
					if (!Keyboard::IsKeyDown(Keys::W) && !Keyboard::IsKeyDown(Keys::A) && !Keyboard::IsKeyDown(Keys::S) && !Keyboard::IsKeyDown(Keys::D) && !Keyboard::IsKeyDown(Keys::Q) && !Keyboard::IsKeyDown(Keys::E))
					{
						m_CameraMovementData.Moving = false;
					}
					else if (!m_CameraMovementData.Moving)
					{
						m_CameraMovementData.Moving = true;
						m_CameraMovementData.MovementTime = 0;
					}
				}
			}));
		
		m_MouseButtonCallback = CreateRef<BrickStacker::MouseButtonCallback>(Mouse::AddMouseButtonCallback([this](MouseButton button, KeyAction action)
			{
				if (!CameraSettings.ListenToMouseInput || !Application::Get().GetFocusedScene()) { return; };
		
				auto Camera = GetWorkingCamera();
				if (Camera == nullptr) { return; }

				if (Camera->Behaviour == CameraBehaviour::Free)
				{
					if (button == MouseButtons::Right && action == KeyAction::Down)
					{
						m_CameraMovementData.Rotating = true;
						Mouse::Get().UpdateImGui = false;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					}
					else if (button == MouseButtons::Right && action == KeyAction::Up)
					{
						m_CameraMovementData.Rotating = false;
						Mouse::Get().UpdateImGui = true;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
				}
				else if (Camera->Behaviour == CameraBehaviour::Orbit)
				{
					if (button == MouseButtons::Middle && action == KeyAction::Down && !Keyboard::IsKeyDown(Keys::LeftShift))
					{
						m_CameraMovementData.Rotating = true;
						Keyboard::Get().UpdateImGui = false;
						Mouse::Get().UpdateImGui = false;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					}
					else if (button == MouseButtons::Middle && action == KeyAction::Up && !m_CameraMovementData.Moving)
					{
						m_CameraMovementData.Rotating = false;
						Keyboard::Get().UpdateImGui = true;
						Mouse::Get().UpdateImGui = true;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
					else if (button == MouseButtons::Middle && action == KeyAction::Down && Keyboard::IsKeyDown(Keys::LeftShift))
					{
						m_CameraMovementData.Moving = true;
						m_CameraMovementData.PrevMousePos = Mouse::GetPosition();
 						Keyboard::Get().UpdateImGui = false;
						Mouse::Get().UpdateImGui = false;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					}
					else if (button == MouseButtons::Middle && action == KeyAction::Up && m_CameraMovementData.Moving)
					{
						m_CameraMovementData.Moving = false;
						m_CameraMovementData.PrevMoving = m_CameraMovementData.Moving;
						Keyboard::Get().UpdateImGui = true;
						Mouse::Get().UpdateImGui = true;
						glfwSetInputMode(Application::Get().GetWindow().GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
				}
			}));
		
		m_MousePositionCallback = CreateRef<BrickStacker::MousePositionCallback>(Mouse::AddMousePositionCallback([this](glm::vec2 pos)
			{
				if (!CameraSettings.ListenToMouseInput || !Application::Get().GetFocusedScene()) { return; };
		
				auto Camera = GetWorkingCamera();
				if (Camera == nullptr) { return; }

				if (Camera->Behaviour == CameraBehaviour::Free)
				{
					if (m_CameraMovementData.Rotating && !m_CameraMovementData.PrevRotating)
					{
						m_CameraMovementData.PrevMousePos = pos;
					}
					else if (m_CameraMovementData.Rotating && m_CameraMovementData.PrevRotating)
					{
						auto currMousePos = pos;
						auto delta = m_CameraMovementData.PrevMousePos - currMousePos;
						Camera->Rotation += glm::vec3(delta.y * (CameraSettings.InvertY - 0.5f) * 2, -delta.x * (CameraSettings.InvertX - 0.5f) * 2, 0) * CameraSettings.Sensitivity;
						Camera->Rotation = { glm::clamp(Camera->Rotation.x, -90.f, 90.f), Camera->Rotation.y, Camera->Rotation.z };
						m_CameraMovementData.PrevMousePos = currMousePos;
					}
				}
		
				if (Camera->Behaviour == CameraBehaviour::Orbit)
				{
					if (m_CameraMovementData.Rotating && !m_CameraMovementData.PrevRotating)
					{
						m_CameraMovementData.PrevMousePos = pos;
		
					}
					else if (m_CameraMovementData.Rotating && m_CameraMovementData.PrevRotating)
					{
						auto currMousePos = pos;
						auto delta = m_CameraMovementData.PrevMousePos - currMousePos;
						Camera->m_OrbitRotation += glm::vec3(delta.y * (CameraSettings.InvertY - 0.5f) * 2, -delta.x * (CameraSettings.InvertX - 0.5f) * 2, 0) * CameraSettings.Sensitivity;
						m_CameraMovementData.PrevMousePos = currMousePos;
					}
		
					if (m_CameraMovementData.Moving && !m_CameraMovementData.PrevMoving)
						m_CameraMovementData.PrevMousePos = pos;
		
					if (m_CameraMovementData.Moving)
					{
						auto currMousePos = pos;
						auto delta = m_CameraMovementData.PrevMousePos - currMousePos;
						m_CameraMovementData.MovementVector = glm::vec3(delta.x * (CameraSettings.InvertY - 0.5f) * 2, delta.y * (CameraSettings.InvertX - 0.5f) * 2, 0) * 0.3f * CameraSettings.Sensitivity;
						m_CameraMovementData.PrevMousePos = currMousePos;
					}
		
					m_CameraMovementData.PrevMoving = m_CameraMovementData.Moving;
				}
		
				m_CameraMovementData.PrevRotating = m_CameraMovementData.Rotating;
			}));
		
		m_MouseScrollCallback = CreateRef<BrickStacker::MouseScrollCallback>(Mouse::AddMouseScrollCallback([this](glm::vec2 pos)
			{
				if (!CameraSettings.ListenToMouseInput || !Application::Get().GetFocusedScene()) { return; };
		
				auto Camera = GetWorkingCamera();
				if (Camera == nullptr) { return; }

				if (Camera->Behaviour == CameraBehaviour::Orbit)
				{
					auto newD = Camera->Distance + pos.y * (CameraSettings.InvertScroll - 0.5f) * 2;
					Camera->Distance = (newD <= 0.0125f) ? 0.0125f : newD;
				}
			}));

	}

	void CameraController::Update(float deltaTime)
	{
		m_CurrScene = Application::Get().GetFocusedScene();

		if (m_PrevScene != m_CurrScene)
		{
			m_CameraMovementData = CameraMovementData();
			m_PrevScene = nullptr;
		}

		if (auto Camera = GetWorkingCamera(); Camera)
		{
			if (Camera->Behaviour == CameraBehaviour::Free)
			{
				m_CameraMovementData.MovementVector =
				{
					Keyboard::IsKeyDown(Keys::A) - Keyboard::IsKeyDown(Keys::D),
					Keyboard::IsKeyDown(Keys::E) - Keyboard::IsKeyDown(Keys::Q),
					Keyboard::IsKeyDown(Keys::W) - Keyboard::IsKeyDown(Keys::S)
				};

				if (m_CurrScene && !m_PrevScene)
					m_CameraMovementData.MovementTime = 0;

				if (m_CameraMovementData.Moving && m_CurrScene && m_PrevScene)
				{
					auto t = m_CameraMovementData.MovementTime - CameraSettings.AccelerateAfter;

					const float x{ glm::radians(Camera->Rotation.x) }, y{ glm::radians(Camera->Rotation.y) }, z{ glm::radians(Camera->Rotation.z) };
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

					auto newMovement = glm::vec3(matrix * glm::vec4(m_CameraMovementData.MovementVector, 1));
					auto useDT = CameraSettings.IgnoreDeltaTime ? 1 : deltaTime;
					auto shiftMultiplier = Keyboard::IsKeyDown(Keys::LeftShift) ? CameraSettings.ShiftMultiplier : 1;
					Camera->Position += newMovement * (shiftMultiplier + (0.5f * CameraSettings.Acceleration * t * t)) * CameraSettings.Speed * useDT;
					m_CameraMovementData.MovementTime += deltaTime;
				}
			}
			else if (Camera->Behaviour == CameraBehaviour::Orbit)
			{
				const float x{ glm::radians(Camera->m_OrbitRotation.x) }, y{ glm::radians(Camera->m_OrbitRotation.y) }, z{ glm::radians(Camera->m_OrbitRotation.z) };
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

				auto newMovement = glm::vec3(matrix * glm::vec4(0, 0, -Camera->Distance, 1));
				auto newTarget = glm::vec3(matrix * glm::vec4(m_CameraMovementData.MovementVector.x, m_CameraMovementData.MovementVector.y, 0, 1));

				Camera->TargetPos = newTarget + Camera->TargetPos;
				Camera->Position = newMovement + Camera->TargetPos;
				m_CameraMovementData.MovementTime += deltaTime;

				m_CameraMovementData.MovementVector = glm::vec3(0);
			}
		}

		for each (auto scene in Application::Get().GetViewports())
		{
			auto Camera = &scene->GetPrimaryCameraComponent();
			if (!Camera)
				continue;

			if (Camera->Type == CameraType::Orthographic)
				Camera->SetOrthographicProjection(Camera->Planes);
			else if (Camera->Type == CameraType::Perspective)
				Camera->SetPerspectiveProjection(Camera->FOV, Camera->Aspect, Camera->Planes.Near, Camera->Planes.Far);

			if (Camera->Behaviour == CameraBehaviour::Free)
				Camera->SetViewYXZ(Camera->Position, Camera->Rotation);
			else if (Camera->Behaviour == CameraBehaviour::Orbit)
				Camera->SetViewRawYXZ(Camera->Position, Camera->m_OrbitRotation);
		}

		m_PrevScene = m_CurrScene;
	}

	Camera* CameraController::GetWorkingCamera()
	{
		if (m_CurrScene != nullptr)
			return &m_CurrScene->GetPrimaryCameraComponent();
		else
			return nullptr;
	}
}
