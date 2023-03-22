#pragma once

#include "glm/glm.hpp"
#include "Keyboard.hpp"

namespace BrickStacker
{
	using MouseButton = uint8_t;
	namespace MouseButtons
	{
		enum : MouseButton
		{
			Zero  = 0,
			One   = 1,
			Two   = 2,
			Three = 3,
			Four  = 4,
			Five  = 5,
			Six   = 6,
			Seven = 7,

			Last   = Seven,
			Left   = Zero,
			Right  = One,
			Middle = Two,
		};
	}

	using MouseButtonFunction = std::function<void(MouseButton, KeyAction)>;
	using MouseButtonCallback = DisconnectableCallback<MouseButtonFunction>;

	using MousePositionFunction = std::function<void(glm::vec2)>;
	using MousePositionCallback = DisconnectableCallback<MousePositionFunction>;

	using MouseScrollFunction = std::function<void(glm::vec2)>;
	using MouseScrollCallback = DisconnectableCallback<MouseScrollFunction>;

	class Mouse
	{
	public:
		static Mouse& Get()
		{
			static Mouse mouse{};
			return mouse;
		}

		static glm::vec2 GetPosition();
		static bool IsButtonDown(const MouseButton button);
		static MouseButtonCallback AddMouseButtonCallback(const MouseButtonFunction& callback);
		static MousePositionCallback AddMousePositionCallback(const MousePositionFunction& callback);
		static MouseScrollCallback AddMouseScrollCallback(const MouseScrollFunction& callback);
		static void Setup();

		bool UpdateImGui = true;
	private:
		Mouse() = default;
		~Mouse() = default;
		static void MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
		static void MousePositionCallbacks(GLFWwindow* window, double x, double y);
		static void MouseScrollCallbacks(GLFWwindow* window, double x, double y);

		std::vector<std::shared_ptr<MouseButtonFunction>> m_MouseButtonCallbacks;
		std::vector<std::shared_ptr<MousePositionFunction>> m_MousePositionCallbacks;
		std::vector<std::shared_ptr<MouseScrollFunction>> m_MouseScrollCallbacks;
	};
}