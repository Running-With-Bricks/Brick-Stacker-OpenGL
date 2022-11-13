#include "BSApplication.hpp"

#include <iostream>

namespace BrickStacker
{
	BSApplication::BSApplication()
	{

	}

	BSApplication::~BSApplication()
	{

	}

	void BSApplication::run()
	{
		glViewport(0, 0, 800, 600);
		glClearColor(0.1f, 0.12f, 0.2f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(m_BSWindow.getWindow());
		while (!m_BSWindow.shouldClose())
		{
			glfwPollEvents();

		}
	}
}