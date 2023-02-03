#pragma once
#include "pch.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

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

		GLFWwindow* GetWindow() const { return m_Window; };

		void SetTitle(const std::string& title)
		{
			m_Title = title;
			glfwSetWindowTitle(m_Window, m_Title.c_str());
		}

		const std::string GetTitle() const
		{
			return m_Title;
		}

		void SetVSync(bool state)
		{
			m_VSync = state;
			glfwSwapInterval(state ? 1 : 0);
		}

		bool IsVSync() const
		{
			return m_VSync;
		}

		bool ShouldClose() const
		{
			return glfwWindowShouldClose(m_Window);
		}

		int GetWindowWidth()
		{
			return m_Width;
		}

		int GetWindowHeight()
		{
			return m_Height;
		}

		float GetAspectRatio() const
		{
			return (float)m_Width / (float)m_Height;
		}

		void Update()
		{
			glfwPollEvents();
			glfwSwapBuffers(m_Window);
		}

	private:
		int m_Width;
		int m_Height;
		std::string m_Title;
		bool m_VSync;

		GLFWwindow* m_Window;
	};
}
