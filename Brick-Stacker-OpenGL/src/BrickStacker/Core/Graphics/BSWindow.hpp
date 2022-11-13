#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>

namespace BrickStacker
{
	class BSWindow
	{
	public:
		//Delete copying constructors
		BSWindow(const BSWindow&) = delete;
		BSWindow& operator=(const BSWindow&) = delete;

		/// <summary>
		/// Create BSWindow
		/// </summary>
		/// <param name="width">Initial width of the window</param>
		/// <param name="height">Initial height of the window</param>
		/// <param name="title">Initial title of the window</param>
		BSWindow(int width, int height, std::string title);
		~BSWindow();

		/// <summary>
		/// Returns GLFWwindow object of the window
		/// </summary>
		/// <returns>GLFWwindow object of the window</returns>
		GLFWwindow* getWindow() { return m_Window; };

		/// <summary>
		/// Set title of the window, honestly idk if I should keep this
		/// </summary>
		/// <param name="title">New title of the window</param>
		void setTitle(std::string title)
		{
			m_Title = title;
			glfwSetWindowTitle(m_Window, m_Title.c_str());
		}
		/// <summary>
		/// Returns current title of the window
		/// </summary>
		/// <returns>Current title of the window</returns>
		std::string getTitle()
		{
			return m_Title;
		}

		/// <summary>
		/// Asks glfw whether window should close
		/// </summary>
		/// <returns>True if window should close, otherwise False</returns>
		bool shouldClose()
		{
			return glfwWindowShouldClose(m_Window);
		}

	private:
		int m_Width;
		int m_Height;
		std::string m_Title;

		GLFWwindow* m_Window;
	};
}
