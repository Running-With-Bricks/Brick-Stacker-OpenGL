#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>

namespace BrickStacker
{
	class Window
	{
	public:
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window(int width, int height, std::string title);
		~Window();

		static void ResizeCallback(GLFWwindow* window, int width, int height);

		//GLFWwindow* GetWindow() { return m_Window; };

		void SetTitle(std::string title)
		{
			m_Title = title;
			glfwSetWindowTitle(m_Window, m_Title.c_str());
		}

		std::string GetTitle() const
		{
			return m_Title;
		}

		void SetVSync(bool state)
		{
			m_VSync = state;
			glfwSwapInterval(state);
		}

		bool IsVSync() const
		{
			return m_VSync;
		}

		void Update()
		{
			glfwPollEvents();
			glfwSwapBuffers(m_Window);
		}

		bool ShouldClose() const
		{
			return glfwWindowShouldClose(m_Window);
		}

	private:
		int m_Width;
		int m_Height;
		std::string m_Title;
		bool m_VSync;

		GLFWwindow* m_Window;
	};
}