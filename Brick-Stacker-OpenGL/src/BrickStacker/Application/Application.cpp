#include "Application.hpp"

#include "glm/glm.hpp"
#include "BrickStacker/Utils/Log.hpp"

namespace BrickStacker
{
	Application::Application()
	{
		const char* vertCode = R"(
			#version 330 core
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;

			out vec4 colour;

			void main()
			{
				gl_Position = vec4(position.x, position.y, position.z, 1.0);
				colour = color;
			}
		)";

		const char* fragCode = R"(
			#version 330 core

			in vec4 colour;

			out vec4 FragColor;
			void main()
			{
				FragColor = colour;
			}
		)";

		m_MainShader.reset(new Shader(vertCode, fragCode));

		m_VertexArray.reset(new VertexArray());
		
		m_VertexBuffer.reset(new VertexBuffer({
			-0.5f, 0.5f, 0, 1, 0, 0, 1,
			0.5f, 0.5f, 0,  0, 1, 0, 1,
			0, -0.5f, 0,    0, 0, 1, 1,
			}));
		BufferLayout layout =
		{
			{ ShaderDataType::Vec3, "position" },
			{ ShaderDataType::Vec4, "color" }
		};
		m_VertexBuffer->SetLayout(layout);
		
		m_IndexBuffer.reset(new IndexBuffer({ 0, 1, 2 }));
		
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		//Pyramid Verts
		std::vector<float> PyramidVerticies =
		{
			 0.0f,  0.5f,  0.0f,  0, 0, 0, 1,
			-0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
			-0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
		};

		//Pyramid Indicies
		std::vector<uint32_t> PyramidIndicies =
		{
			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 1,
			1, 2, 3,
			3, 1, 4
		};

		//Specifying Vertex properties
		//Vec3 of floats for the position and Vec4 of floats for Color
		BufferLayout PyramidLayout =
		{
			{ ShaderDataType::Vec3, "Position" },
			{ ShaderDataType::Vec4, "Color" }
		};

		//Creating objects which will hold pyramid data
		m_PyramidVertexArray.reset(new VertexArray());
		m_PyramidVertexBuffer.reset(new VertexBuffer(PyramidVerticies));
		m_PyramidIndexBuffer.reset(new IndexBuffer(PyramidIndicies));

		//Setting the layout
		m_PyramidVertexBuffer->SetLayout(PyramidLayout);

		//Binding VertexBuffer and IndexBuffer to VertexArray
		m_PyramidVertexArray->AddVertexBuffer(m_PyramidVertexBuffer);
		m_PyramidVertexArray->SetIndexBuffer(m_PyramidIndexBuffer);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		glClearColor(0.1f, 0.12f, 0.2f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		m_Window.Update();
		m_Window.SetVSync(true);

		m_Discord.SetActivityDetails("Brick Stacking, Brick Build Together");
		m_Discord.SetActivityState("Part Piece Construct Make Create Set.");

		while (!m_Window.ShouldClose())
		{
			m_Discord.Update();

			Draw();
		}
	}

	void Application::Draw()
	{
		glClearColor(0.11f, 0.12f, 0.18f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_MainShader->Bind();

		m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

		//Binding VertexArray to specify that we are using it rn
		//And then drawing
		m_PyramidVertexArray->Bind();
		glDrawElements(GL_TRIANGLES, m_PyramidVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

		m_Window.Update();
	}
}