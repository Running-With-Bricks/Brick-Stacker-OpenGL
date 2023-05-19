#pragma once
#include "pch.hpp"

#include "Scene.hpp"

namespace BrickStacker
{
	class CameraController
	{
	public:
		CameraController(const CameraController&) = delete;
		CameraController& operator=(const CameraController&) = delete;

		static CameraController& Get()
		{
			static CameraController CameraController{};
			return CameraController;
		}

		void Update(float deltaTime);
		Camera* GetWorkingCamera();

		struct CameraSettings
		{
			bool ListenToKeyboardInput = true;
			bool ListenToMouseInput = true;

			float AccelerateAfter = 5;
			float Acceleration = 0.05f;
			float Speed = 15.f;
			float ShiftMultiplier = 6;
			bool IgnoreDeltaTime = false;

			float Sensitivity = 0.4f;
			float ScrollSensitivity = 2.5f;
			bool InvertX = false;
			bool InvertY = false;
			bool InvertScroll = false;
		} CameraSettings;

	private:
		CameraController();
		~CameraController() = default;
		
		Ref<KeyCallback> m_KeyboardCallback;
		Ref<MouseButtonCallback> m_MouseButtonCallback;
		Ref<MousePositionCallback> m_MousePositionCallback;
		Ref<MouseScrollCallback> m_MouseScrollCallback;

		struct CameraMovementData
		{
			bool Moving = false;
			bool PrevMoving = false;
			float MovementTime;
			glm::vec3 MovementVector{ 0, 0, 0 };
			bool Rotating = false;
			bool PrevRotating = false;
			glm::vec2 PrevMousePos{ 0 };
		} m_CameraMovementData;

		Ref<Scene> m_PrevScene;
		Ref<Scene> m_CurrScene;
	};
}