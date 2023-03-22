#include "pch.hpp"
#include "Mouse.hpp"

#include "BrickStacker/Application/Application.hpp"

namespace BrickStacker
{
	glm::vec2 Mouse::GetPosition()
	{
		double x, y;
		glfwGetCursorPos(Application::Get().GetWindow().GetWindowHandle(), &x, &y);
		return glm::vec2(x, y);
	}

	bool Mouse::IsButtonDown(const MouseButton button)
	{
		auto* window = Application::Get().GetWindow().GetWindowHandle();
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	};

	MouseButtonCallback Mouse::AddMouseButtonCallback(const MouseButtonFunction& callback)
	{
		Mouse::Get().m_MouseButtonCallbacks.push_back(std::make_shared<MouseButtonFunction>(callback));
		MouseButtonCallback disconnect{ Mouse::Get().m_MouseButtonCallbacks[Mouse::Get().m_MouseButtonCallbacks.size() - 1], Mouse::Get().m_MouseButtonCallbacks };
		return disconnect;
	}

	MousePositionCallback Mouse::AddMousePositionCallback(const MousePositionFunction& callback)
	{
		Mouse::Get().m_MousePositionCallbacks.push_back(std::make_shared<MousePositionFunction>(callback));
		MousePositionCallback disconnect{ Mouse::Get().m_MousePositionCallbacks[Mouse::Get().m_MousePositionCallbacks.size() - 1], Mouse::Get().m_MousePositionCallbacks };
		return disconnect;
	}

	MouseScrollCallback Mouse::AddMouseScrollCallback(const MousePositionFunction& callback)
	{
		Mouse::Get().m_MouseScrollCallbacks.push_back(std::make_shared<MouseScrollFunction>(callback));
		MouseScrollCallback disconnect{ Mouse::Get().m_MouseScrollCallbacks[Mouse::Get().m_MouseScrollCallbacks.size() - 1], Mouse::Get().m_MouseScrollCallbacks };
		return disconnect;
	}

	void Mouse::MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods)
	{
		if (Mouse::Get().UpdateImGui)
			ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
		for (auto& callback : Mouse::Get().m_MouseButtonCallbacks)
			(*callback)(static_cast<MouseButton>(button), static_cast<KeyAction>(action));
	}

	void Mouse::MousePositionCallbacks(GLFWwindow* window, double x, double y)
	{
		if (Mouse::Get().UpdateImGui)
			ImGui_ImplGlfw_CursorPosCallback(window, x, y);
		for (auto& callback : Mouse::Get().m_MousePositionCallbacks)
			(*callback)(glm::vec2(x, y));
	}

	void Mouse::MouseScrollCallbacks(GLFWwindow* window, double x, double y)
	{
		if (Mouse::Get().UpdateImGui)
			ImGui_ImplGlfw_ScrollCallback(window, x, y);
		for (auto& callback : Mouse::Get().m_MouseScrollCallbacks)
			(*callback)(glm::vec2(x, y));
	}

	void Mouse::Setup()
	{
		Mouse::Get();
		glfwSetMouseButtonCallback(Application::Get().GetWindow().GetWindowHandle(), MouseButtonCallbacks);
		glfwSetCursorPosCallback(Application::Get().GetWindow().GetWindowHandle(), MousePositionCallbacks);
		glfwSetScrollCallback(Application::Get().GetWindow().GetWindowHandle(), MouseScrollCallbacks);
	}
}