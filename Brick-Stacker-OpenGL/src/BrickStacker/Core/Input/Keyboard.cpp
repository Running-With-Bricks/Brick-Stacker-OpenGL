#include "pch.hpp"
#include "Keyboard.hpp"

#include "BrickStacker/Application/Application.hpp"

namespace BrickStacker
{
	bool Keyboard::IsKeyDown(const Keycode key)
	{
		auto* window = Application::Get().GetWindow().GetWindowHandle();
		auto state = glfwGetKey(window, static_cast<int>(key));
		return state == GLFW_PRESS;
	}

	KeyCallback Keyboard::AddCallback(const KeyFunction& callback)
	{
		Keyboard::Get().m_Callbacks.push_back(std::make_shared<KeyFunction>(callback));
		KeyCallback disconnect{ Keyboard::Get().m_Callbacks[Keyboard::Get().m_Callbacks.size() - 1], Keyboard::Get().m_Callbacks };
		return disconnect;
	}
	
	void Keyboard::KeyCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (Keyboard::Get().UpdateImGui)
			ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
		for (auto& callback : Keyboard::Get().m_Callbacks)
			(*callback)(static_cast<Keycode>(key), static_cast<KeyAction>(action));
	}

	void Keyboard::Setup()
	{
		Keyboard::Get();
		glfwSetKeyCallback(Application::Get().GetWindow().GetWindowHandle(), KeyCallbacks);
	}
}