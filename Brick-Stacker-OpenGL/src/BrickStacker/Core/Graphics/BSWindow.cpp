#include "BSWindow.hpp"

namespace BrickStacker
{
	BSWindow::BSWindow(int width, int height, std::string title)
		: m_Width{width}, m_Height{height}, m_Title{title}
	{
		glfwInit();

		//Set the glfw version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//opengl profile thingy
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//Do not resize, will take care of that later
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
		glfwSetWindowUserPointer(m_Window, this);
		glfwMakeContextCurrent(m_Window);

		gladLoadGL();
	}

	BSWindow::~BSWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}
