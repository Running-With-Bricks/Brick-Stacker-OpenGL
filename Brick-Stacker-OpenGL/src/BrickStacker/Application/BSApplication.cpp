#include "BSApplication.hpp"

#include "DiscordGameSDK/discord.h"

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
		const char* vertCode = "#version 330 core\n"
			"layout(location = 0) in vec3 position;\n"
			"void main()\n"
			"{\n"
			"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
			"}\n\0";

		const char* fragCode = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"FragColor = vec4(1, 0, 0, 1);\n"
			"}\n\0";

		float vertices[] =
		{
			-0.5f, 0.5f, 0,
			0.5f, 0.5f, 0,
			0, -0.5f, 0
		};

		uint32_t indicies[] =
		{
			0, 1, 2
		};

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertCode, NULL);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragCode, NULL);
		glCompileShader(fragmentShader);

		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		GLuint VAO, VBO, EBO;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		glViewport(0, 0, 800, 600);
		glClearColor(0.1f, 0.12f, 0.2f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(m_BSWindow.getWindow());

		glfwSwapInterval(1);

		while (!m_BSWindow.shouldClose())
		{
			glfwPollEvents();
			m_Discord.Update();
			
			glClearColor(0.11f, 0.12f, 0.18f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

			glfwSwapBuffers(m_BSWindow.getWindow());
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);
	}
}