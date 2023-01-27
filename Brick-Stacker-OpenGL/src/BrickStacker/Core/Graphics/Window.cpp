#include "Window.hpp"

#include "BrickStacker/Application/Application.hpp"

namespace BrickStacker
{
	Window::Window(int width, int height, std::string title)
		: m_Width{ width }, m_Height{ height }, m_Title{ title }, m_VSync{ false }
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		//glfwWindowHint(GLFW_SAMPLES, 4);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

		glfwSetWindowUserPointer(m_Window, this);
		glfwMakeContextCurrent(m_Window);

		glfwMaximizeWindow(m_Window);
		glfwSetFramebufferSizeCallback(m_Window, ResizeCallback);

		glfwSetWindowSizeLimits(m_Window, 266, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

		glfwSwapInterval((int)m_VSync);

		gladLoadGL();
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Window::ResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto m_Window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		m_Window->m_Width = width;
		m_Window->m_Height = height;

		Application::Get().Draw();
	}
}
