#include "Application.hpp"

#include "BrickStacker/Utils/Log.hpp"

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_glfw.h"
#include "ImGui/backends/imgui_impl_opengl3.h"

namespace BrickStacker
{
	Application::Application()
	{
		const char* vertCode = R"(
			#version 330 core
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;

			uniform mat4 u_Transform;
			uniform mat4 u_ViewMatrix;
			uniform mat4 u_ProjectionMatrix;

			out vec4 colour;

			void main()
			{
				gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_Transform * vec4(position, 1.0);
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
		
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;

		vertexBuffer.reset(new VertexBuffer({
			-0.5f, 0.5f, 0, 1, 0, 0, 1,
			0.5f, 0.5f, 0,  0, 1, 0, 1,
			0, -0.5f, 0,    0, 0, 1, 1,
			}));
		BufferLayout layout =
		{
			{ ShaderDataType::Vec3, "position" },
			{ ShaderDataType::Vec4, "color" }
		};
		vertexBuffer->SetLayout(layout);
		
		indexBuffer.reset(new IndexBuffer({ 0, 1, 2 }));
		
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		//Cube Verts
		std::vector<float> CubeVerticies =
		{
		//Top
			-0.5f,  0.5f,  0.5f,  1, 1, 1, 1,
			 0.5f,  0.5f,  0.5f,  1, 1, 1, 1,
			 0.5f,  0.5f, -0.5f,  1, 1, 1, 1,
			-0.5f,  0.5f, -0.5f,  1, 1, 1, 1,
		//Bottom
			-0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
			-0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
		//Left
			-0.5f,  0.5f,  0.5f,  1, 1, 1, 1,
			-0.5f,  0.5f, -0.5f,  1, 1, 1, 1,
			-0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
			-0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
		//Right
			 0.5f,  0.5f,  0.5f,  1, 1, 1, 1,
			 0.5f,  0.5f, -0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
		//Front
			 0.5f,  0.5f, -0.5f,  1, 1, 1, 1,
			-0.5f,  0.5f, -0.5f,  1, 1, 1, 1,
			-0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
		//Back
			 0.5f,  0.5f,  0.5f,  1, 1, 1, 1,
			-0.5f,  0.5f,  0.5f,  1, 1, 1, 1,
			-0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
		};

		//Cube Indicies
		std::vector<uint32_t> CubeIndicies =
		{
			1, 2, 0,
			3, 0, 2,

			4, 6, 5,
			6, 4, 7,

			9, 10, 8,
			11, 8, 10,

			12, 14, 13,
			14, 12, 15,

			16, 18, 17,
			18, 16, 19,

			21, 22, 20,
			23, 20, 22,
		};

		//Skybox Verts
		std::vector<float> SkyboxVerticies =
		{
		//Top
			-0.5f,  0.5f,  0.5f,  .1f, .14f, .9f, 1,
			 0.5f,  0.5f,  0.5f,  .1f, .14f, .9f, 1,
			 0.5f,  0.5f, -0.5f,  .1f, .14f, .9f, 1,
			-0.5f,  0.5f, -0.5f,  .1f, .14f, .9f, 1,
		//Bottom
			-0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
			-0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
		//Left
			-0.5f,  0.5f,  0.5f,  .1f, .14f, .9f, 1,
			-0.5f,  0.5f, -0.5f,  .1f, .14f, .9f, 1,
			-0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
			-0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
		//Right
			 0.5f,  0.5f,  0.5f,  .1f, .14f, .9f, 1,
			 0.5f,  0.5f, -0.5f,  .1f, .14f, .9f, 1,
			 0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
		//Front
			 0.5f,  0.5f, -0.5f,  .1f, .14f, .9f, 1,
			-0.5f,  0.5f, -0.5f,  .1f, .14f, .9f, 1,
			-0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f, -0.5f,  1, 1, 1, 1,
		//Back
			 0.5f,  0.5f,  0.5f,  .1f, .14f, .9f, 1,
			-0.5f,  0.5f,  0.5f,  .1f, .14f, .9f, 1,
			-0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
			 0.5f, -0.5f,  0.5f,  1, 1, 1, 1,
		};

		//Skybox Indicies
		std::vector<uint32_t> SkyboxIndicies =
		{
			0, 2, 1,
			2, 0, 3,

			5, 6, 4,
			7, 4, 6,

			8, 10, 9,
			10, 8, 11,

			13, 14, 12,
			15, 12, 14,

			17, 18, 16,
			19, 16, 18,

			20, 22, 21,
			22, 20, 23,
		};

		//Specifying Vertex properties
		//Vec3 of floats for the position and Vec4 of floats for Color
		BufferLayout CubeLayout =
		{
			{ ShaderDataType::Vec3, "Position" },
			{ ShaderDataType::Vec4, "Color" }
		};

		//Creating objects which will hold cube data
		std::shared_ptr<VertexBuffer> cubeVertexBuffer;
		std::shared_ptr<IndexBuffer> cubeIndexBuffer;

		m_CubeVertexArray.reset(new VertexArray());
		cubeVertexBuffer.reset(new VertexBuffer(CubeVerticies));
		cubeIndexBuffer.reset(new IndexBuffer(CubeIndicies));

		//Setting the layout
		cubeVertexBuffer->SetLayout(CubeLayout);

		//Binding VertexBuffer and IndexBuffer to VertexArray
		m_CubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
		m_CubeVertexArray->SetIndexBuffer(cubeIndexBuffer);

		//Creating objects which will hold cube data
		std::shared_ptr<VertexBuffer> skyboxVertexBuffer;
		std::shared_ptr<IndexBuffer> skyboxIndexBuffer;

		m_SkyboxVertexArray.reset(new VertexArray());
		skyboxVertexBuffer.reset(new VertexBuffer(SkyboxVerticies));
		skyboxIndexBuffer.reset(new IndexBuffer(SkyboxIndicies));

		//Setting the layout
		skyboxVertexBuffer->SetLayout(CubeLayout);

		//Binding VertexBuffer and IndexBuffer to VertexArray
		m_SkyboxVertexArray->AddVertexBuffer(skyboxVertexBuffer);
		m_SkyboxVertexArray->SetIndexBuffer(skyboxIndexBuffer);

		m_Camera.reset(new Camera());
		m_Camera->Position = { 0, 0, -2 };
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		RenderCommand::SetClearColor({ 0.1f, 0.12f, 0.2f, 1 });
		RenderCommand::Clear();

		m_Window.Update();
		m_Window.SetVSync(true);

		m_Discord.SetActivityDetails("Brick Stacking, Brick Build Together");
		m_Discord.SetActivityState("Part Piece Construct Make Create Set.");

		m_Renderer.SetCamera(m_Camera);

		while (!m_Window.ShouldClose())
		{
			m_Discord.Update();

			Draw();
		}
	}

	void Application::Draw()
	{
		m_Camera->Aspect = m_Window.GetAspectRatio();
		m_Camera->Update();

		RenderCommand::Clear();

		m_Renderer.Submit(m_VertexArray, m_MainShader);
		m_Renderer.Submit(m_CubeVertexArray, m_MainShader, { .1f, 2, .5f });
		m_Renderer.Submit(m_SkyboxVertexArray, m_MainShader, { 32, 32, 32 });

		m_ImGui.BeginFrame();
		ImGui::ShowDemoWindow();
		
		ImGui::Begin("Viewport");
		ImGui::End();
		
		ImGui::Begin("Explorer");
		ImGui::End();
		
		ImGui::Begin("Properties");
		ImGui::End();
		
		int type = (int)m_Camera->Type;
		int behaviour = (int)m_Camera->Behaviour;

		ImGui::Begin("Debug");
		ImGui::Combo("CamType", &type, "Perspective\0Orthographic\0\0");
		ImGui::Combo("CamBehaviour", &behaviour, "None\0Free\0Orbit\0\0");
		ImGui::DragFloat("CamFOV", &m_Camera->FOV, .1f, 0.1f, 120);
		ImGui::DragFloat3("CamPos", &m_Camera->Position.x, .1f);
		ImGui::DragFloat3("CamRot", &m_Camera->Rotation.x, .5f);
		ImGui::DragFloat3("CamTar", &m_Camera->TargetPos.x, .1f);

		m_Camera->Type = (CameraType)type;
		m_Camera->Behaviour = (CameraBehaviour)behaviour;
		ImGui::End();
		
		m_ImGui.EndFrame();

		m_Window.Update();
	}
}