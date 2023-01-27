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
			layout(location = 2) in vec4 transformCol1;
			layout(location = 3) in vec4 transformCol2;
			layout(location = 4) in vec4 transformCol3;
			layout(location = 5) in vec4 transformCol4;

			uniform mat4 u_ViewMatrix;
			uniform mat4 u_ProjectionMatrix;

			out vec4 colour;

			void main()
			{
				mat4 transform;
				transform[0] = transformCol1;
				transform[1] = transformCol2;
				transform[2] = transformCol3;
				transform[3] = transformCol4;

				gl_Position = u_ProjectionMatrix * u_ViewMatrix * transform * vec4(position, 1.0);
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

		m_MainShader = Shader::Create(vertCode, fragCode);

		m_VertexArray = VertexArray::Create();
		
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;

		vertexBuffer = VertexBuffer::Create({
			-0.5f, 0.5f, 0, 1, 0, 0, 1,
			0.5f, 0.5f, 0,  0, 1, 0, 1,
			0, -0.5f, 0,    0, 0, 1, 1,
			});
		BufferLayout layout =
		{
			{ ShaderDataType::Vec3, "position" },
			{ ShaderDataType::Vec4, "color" }
		};
		vertexBuffer->SetLayout(layout);
		
		indexBuffer = IndexBuffer::Create({ 0, 1, 2 });
		
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

		BufferLayout InstancedLayout =
		{
			{ ShaderDataType::Vec4, "Transform", true },
			{ ShaderDataType::Vec4, "Transform", true },
			{ ShaderDataType::Vec4, "Transform", true },
			{ ShaderDataType::Vec4, "Transform", true },
		};

		//Creating objects which will hold cube data
		glm::mat4 transform{ 1.0f };
		std::vector<float> vbTransform;

		transform = glm::translate(transform, { 2, 0, 1 });
		transform = glm::scale(transform, { 1, 2, 1 });

		for (uint32_t i = 0; i < 4; i++)
		{
			for (uint32_t j = 0; j < 4; j++)
			{
				vbTransform.push_back(transform[i][j]);
			}
		}

		BS_INFO("{0} {1} {2} {3}", transform[0].x, transform[0].y, transform[0].z, transform[0].w);
		BS_INFO("{0} {1} {2} {3}", transform[1].x, transform[1].y, transform[1].z, transform[1].w);
		BS_INFO("{0} {1} {2} {3}", transform[2].x, transform[2].y, transform[2].z, transform[2].w);
		BS_INFO("{0} {1} {2} {3}", transform[3].x, transform[3].y, transform[3].z, transform[3].w);

		Ref<VertexBuffer> cubeVertexBuffer = VertexBuffer::Create(CubeVerticies);
		Ref<VertexBuffer> cubeInstancedVertexBuffer = VertexBuffer::Create(vbTransform, GL_STREAM_DRAW);
		Ref<IndexBuffer> cubeIndexBuffer = IndexBuffer::Create(CubeIndicies);

		m_CubeVertexArray = VertexArray::Create();

		//Setting the layout
		cubeVertexBuffer->SetLayout(CubeLayout);
		cubeInstancedVertexBuffer->SetLayout(InstancedLayout);

		//Binding VertexBuffer and IndexBuffer to VertexArray
		m_CubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
		m_CubeVertexArray->AddVertexBuffer(cubeInstancedVertexBuffer);
		m_CubeVertexArray->SetIndexBuffer(cubeIndexBuffer);

		//Creating objects which will hold cube data
		Ref<VertexBuffer> skyboxVertexBuffer;
		Ref<IndexBuffer> skyboxIndexBuffer;

		//m_SkyboxVertexArray = VertexArray::Create();
		//skyboxVertexBuffer = VertexBuffer::Create(SkyboxVerticies);
		//skyboxIndexBuffer = IndexBuffer::Create(SkyboxIndicies);

		//Setting the layout
		//skyboxVertexBuffer->SetLayout(CubeLayout);

		//Binding VertexBuffer and IndexBuffer to VertexArray
		//m_SkyboxVertexArray->AddVertexBuffer(skyboxVertexBuffer);
		//m_SkyboxVertexArray->SetIndexBuffer(skyboxIndexBuffer);

		m_Camera = Camera::Create();
		m_Camera->Position = { 0, 0, -2 };

		FramebufferSpecifications fbSpecs;
		fbSpecs.Width = 1024;
		fbSpecs.Height = 1024;
		m_Framebuffer = Framebuffer::Create(fbSpecs); //New
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
		RenderCommand::Clear();

		m_ImGui.BeginFrame();
		ImGui::DockSpaceOverViewport();

		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		auto viewportSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportSize) && viewportSize.x != 0 && viewportSize.y != 0)
		{
			m_ViewportSize = { viewportSize.x, viewportSize.y };
			glViewport(0, 0, (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		auto textureID = m_Framebuffer->GetColorAttachmentRendererID();

		if (textureID)
			ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

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

		if (m_ViewportSize.x != 0 && m_ViewportSize.y != 0)
			m_Camera->Aspect = m_ViewportSize.x / m_ViewportSize.y;
		m_Camera->Update();

		m_Framebuffer->Bind();
		RenderCommand::Clear();
		m_Renderer.Submit(m_VertexArray, m_MainShader, 1);
		m_Renderer.Submit(m_CubeVertexArray, m_MainShader, 1);
		//m_Renderer.Submit(m_SkyboxVertexArray, m_MainShader, { 32, 32, 32 });
		m_Framebuffer->Unbind();

		m_Window.Update();
	}
}