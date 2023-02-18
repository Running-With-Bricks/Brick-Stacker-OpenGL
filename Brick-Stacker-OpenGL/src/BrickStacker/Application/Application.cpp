#include "pch.hpp"
#include "Application.hpp"

#include <algorithm>

namespace std
{
	void split_str(const std::string& str, const char delim, std::vector<std::string>& out)
	{
		// create a stream from the string  
		std::stringstream s(str);

		std::string s2;
		while (std::getline(s, s2, delim))
		{
			out.push_back(s2); // store the string in s2  
		}
	}
}

namespace BrickStacker
{
	Application::Application()
	{
		const char* vertCode = R"(
			#version 460 core
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec2 texCoord;
			layout(location = 2) in float texIndex;
			layout(location = 3) in vec4 color;
			layout(location = 4) in vec2 tiling;
			layout(location = 5) in vec3 transformCol1;
			layout(location = 6) in vec3 transformCol2;
			layout(location = 7) in vec3 transformCol3;
			layout(location = 8) in vec3 transformCol4;

			uniform mat4 u_ViewMatrix;
			uniform mat4 u_ProjectionMatrix;

			struct VertexOutput
			{
				vec4 Color;
				vec2 TexCoord;
				float TexIndex;
				vec2 TilingFactor;
			};
			
			layout (location = 0) out VertexOutput Output;

			void main()
			{
				mat4 transform;
				transform[0] = vec4(transformCol1, 0);
				transform[1] = vec4(transformCol2, 0);
				transform[2] = vec4(transformCol3, 0);
				transform[3] = vec4(transformCol4, 1);

				gl_Position = u_ProjectionMatrix * u_ViewMatrix * transform * vec4(position, 1.0);
				Output.Color = color;
				Output.TexCoord = texCoord;
				Output.TexIndex = int(texIndex);
				Output.TilingFactor = tiling;
			}
		)";

		const char* fragCode = R"(
			#version 460 core

			struct VertexOutput
			{
				vec4 Color;
				vec2 TexCoord;
				float TexIndex;
				vec2 TilingFactor;
			};
			
			layout (location = 0) in VertexOutput Input;

			layout (binding = 0) uniform sampler2D Textures[2];

			out vec4 FragColor;
			void main()
			{
				FragColor = Input.Color;
				switch(int(Input.TexIndex))
				{
					case 1: FragColor *= texture(Textures[int(Input.TexIndex)-1], Input.TexCoord*Input.TilingFactor)*Input.Color; break;
					case 2: FragColor *= texture(Textures[int(Input.TexIndex)-1], Input.TexCoord*Input.TilingFactor)*Input.Color; break;
				}
			}
		)";

		m_MainShader = Shader::Create(vertCode, fragCode);

		//Cube Verts
		std::vector<float> CubeVerticies =
		{
		//Top
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1,
		//Bottom
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 2,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 2,
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 2,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 2,
		//Left
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0,
		//Right
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0,
		//Front
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0,
		//Back
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0,
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


		//Specifying Vertex properties
		//Vec3 of floats for the position and Vec4 of floats for Color
		BufferLayout CubeLayout =
		{
			{ ShaderDataType::Vec3, "Position" },
			{ ShaderDataType::Vec2, "TexCoord" },
			{ ShaderDataType::Float, "TexIndex" },
		};

		BufferLayout InstancedLayout =
		{
			{ ShaderDataType::Vec4, "Color", true },
			{ ShaderDataType::Vec2, "TilingFactor", true},
			{ ShaderDataType::Vec3, "Transform1", true },
			{ ShaderDataType::Vec3, "Transform2", true },
			{ ShaderDataType::Vec3, "Transform3", true },
			{ ShaderDataType::Vec3, "Transform4", true },
		};

		//Creating objects which will hold cube data

		Ref<VertexBuffer> cubeVertexBuffer = VertexBuffer::Create(CubeVerticies);
		Ref<VertexBuffer> cubeInstancedVertexBuffer = VertexBuffer::Create({}, GL_STREAM_DRAW);
		Ref<IndexBuffer> cubeIndexBuffer = IndexBuffer::Create(CubeIndicies);

		m_CubeVertexArray = VertexArray::Create();

		//Setting the layout
		cubeVertexBuffer->SetLayout(CubeLayout);
		cubeInstancedVertexBuffer->SetLayout(InstancedLayout);

		//Binding VertexBuffer and IndexBuffer to VertexArray
		m_CubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
		m_CubeVertexArray->AddVertexBuffer(cubeInstancedVertexBuffer);
		m_CubeVertexArray->SetIndexBuffer(cubeIndexBuffer);


		m_Camera = Camera::Create();
		m_Camera->Position = { 0, 1, 2 };

		FramebufferSpecifications fbSpecs;
		fbSpecs.Width = 1024;
		fbSpecs.Height = 1024;
		m_Framebuffer = Framebuffer::Create(fbSpecs);

		m_Texture = Texture2D::Create("assets/images/stud_top.png");
		m_Texture2 = Texture2D::Create("assets/images/stud_bottom.png");

		std::ifstream mapFile("Map.brk", std::ios::in);

		m_Timer.Reset();
		if (mapFile.is_open())
		{
			std::string line;

			bool wasAmbientEncountered{ false };
			bool wasBaseplateSizeEncountered{ false };
			bool completedSetup{ false };
			int currentEntityId = 1;

			while (std::getline(mapFile, line))
			{
				std::vector<std::string> separated;
				std::split_str(line, ' ', separated);

				if (!completedSetup)
				{
					if (separated.size() == 3)
					{
						if (!wasAmbientEncountered)
						{
							wasAmbientEncountered = true;
							//Ambient Color
						}
						else
						{
							//Sky Color
							glm::vec3 color{ 0 };

							std::sscanf(separated[2].c_str(), "%f", &color.r);
							std::sscanf(separated[1].c_str(), "%f", &color.g);
							std::sscanf(separated[0].c_str(), "%f", &color.b);
						}
					}
					else if (separated.size() == 4)
					{
						//Baseplate Color + Alpha
						glm::vec3 color{ 0 };

						std::sscanf(separated[2].c_str(), "%f", &color.r);
						std::sscanf(separated[1].c_str(), "%f", &color.g);
						std::sscanf(separated[0].c_str(), "%f", &color.b);
					}
					else if (separated.size() == 1)
					{
						if (!wasBaseplateSizeEncountered)
						{
							//Baseplate Size
							wasBaseplateSizeEncountered = true;

							float r = 0;
							std::sscanf(separated[0].c_str(), "%f", &r);
						}
						else
						{
							//Sun Intensity
							completedSetup = true;
						}
					}
				}
				else if (separated.size() == 10)
				{
					//Brick Start
					glm::vec3 position{ 0 };
					glm::vec3 scale{ 0 };
					glm::vec4 color{ 0, 0, 0, 1 };

					std::sscanf(separated[0].c_str(), "%f", &position.x);
					std::sscanf(separated[2].c_str(), "%f", &position.y);
					std::sscanf(separated[1].c_str(), "%f", &position.z);

					std::sscanf(separated[3].c_str(), "%f", &scale.x);
					std::sscanf(separated[5].c_str(), "%f", &scale.y);
					std::sscanf(separated[4].c_str(), "%f", &scale.z);

					std::sscanf(separated[6].c_str(), "%f", &color.r);
					std::sscanf(separated[7].c_str(), "%f", &color.g);
					std::sscanf(separated[8].c_str(), "%f", &color.b);
					std::sscanf(separated[9].c_str(), "%f", &color.a);

					position += scale * .5f;

					auto brick = Brick::Create("");
					brick->Position = position;
					brick->Scale = scale;
					brick->Color = color;

					currentEntityId = m_Bricks.size();
					m_Bricks.push_back(brick);
				}
				else if (separated.size() > 0 && separated[0].rfind("\t+", 0) == 0)
				{
					//we affect the m_Entities[currentEntityId]
					//if line starts with "    +"
					//example: "    +NAME Brick2"
					if (separated[0].rfind("\t+NAME", 0) == 0)
					{
						std::string name = "";
						for (int i = 1; i < separated.size(); i++)
						{
							name += (i == 1 ? "" : " ") + separated[i];
						}

						m_Bricks[currentEntityId]->Name = name;
					}
					else if (separated[0].rfind("\t+ROT", 0) == 0)
					{
						int rot{ 0 };
						std::sscanf(separated[1].c_str(), "%d", &rot);
						m_Bricks[currentEntityId]->Rotation.y = static_cast<float>(rot);

						if ((rot % 180) != 0)
						{
							glm::vec3 orgPos = m_Bricks[currentEntityId]->Position - m_Bricks[currentEntityId]->Scale * .5f;
							float tempXScale = static_cast<float>(m_Bricks[currentEntityId]->Scale.x);
							m_Bricks[currentEntityId]->Scale.x = m_Bricks[currentEntityId]->Scale.z;
							m_Bricks[currentEntityId]->Scale.z = tempXScale;
						}
					}
				}
			}
		}
		m_SortedBricks = m_Bricks;
		m_Timer.Stop();
		m_LoadTime = m_Timer.Elapsed();

		mapFile.close();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		RenderCommand::SetClearColor({ .5f, .5f, .5f, 1 });
		RenderCommand::Clear();

		m_Window.Update();
		m_Window.SetVSync(true);

		m_Discord.SetActivityDetails("Brick Stacking, Brick Build Together");
		m_Discord.SetActivityState("Part Piece Construct Make Create Set.");

		m_Renderer.SetCamera(m_Camera);

		while (!m_Window.ShouldClose())
		{
			m_Discord.Update();

			m_Timer.Reset();
			Draw();
			m_Timer.Stop();

			m_Window.Update();
		}

		ImGui::SaveIniSettingsToDisk("imgui.ini");
	}

	 void Application::updateBricksInstancedData()
	{
		std::vector<float> instancedData;

		std::sort(m_SortedBricks.begin(), m_SortedBricks.end(), [m_Camera = m_Camera](const Ref<Brick>& a, const Ref<Brick>& b)
			{
				if ((a->Color.a == 1.0f) && (b->Color.a != 1.0f)) {
					// If a has alpha 1 and b doesn't, a comes first
					return true;
				}
				else if (a->Color.a != 1.0f && b->Color.a == 1.0f) {
					// If b has alpha 1 and a doesn't, b comes first
					return false;
				}
				else {
					// If both have alpha 1 or both don't have alpha 1, compare by position length
					return glm::length(m_Camera->Position - a->Position) > glm::length(m_Camera->Position - b->Position);
				}
			}
		);

		for (size_t i = 0; i < m_SortedBricks.size(); i++)
		{
			const auto& brick = m_SortedBricks[i];
			if (!brick->Visible)
				continue;

			const float x{ glm::radians(brick->Rotation.x) }, y{ glm::radians(brick->Rotation.y) }, z{ glm::radians(brick->Rotation.z) };
			const float c3 = glm::cos(z);
			const float s3 = glm::sin(z);
			const float c2 = glm::cos(x);
			const float s2 = glm::sin(x);
			const float c1 = glm::cos(y);
			const float s1 = glm::sin(y);
			glm::mat4 brickMatrix = glm::mat4{
			{
				brick->Scale.x * (c1 * c3 + s1 * s2 * s3),
				brick->Scale.x * (c2 * s3),
				brick->Scale.x * (c1 * s2 * s3 - c3 * s1),
				0.0f,
			},
			{
				brick->Scale.y * (c3 * s1 * s2 - c1 * s3),
				brick->Scale.y * (c2 * c3),
				brick->Scale.y * (c1 * c3 * s2 + s1 * s3),
				0.0f,
			},
			{
				brick->Scale.z * (c2 * s1),
				brick->Scale.z * (-s2),
				brick->Scale.z * (c1 * c2),
				0.0f,
			},
			{brick->Position.x, brick->Position.y, brick->Position.z, 1.0f} };

			for (size_t x = 0; x < 4; x++)
			{
				instancedData.push_back(brick->Color[x]);
			}
			instancedData.push_back(brick->Scale.x);
			instancedData.push_back(brick->Scale.z);
			for (size_t x = 0; x < 4; x++)
			{
				for (size_t y = 0; y < 3; y++)
				{
					instancedData.push_back(brickMatrix[x][y]);
				}
			}
		}

		m_CubeVertexArray->GetVertexBuffers()[1]->UpdateBuffer(instancedData, GL_STREAM_DRAW);
	}

	void Application::Draw()
	{
		updateBricksInstancedData();

		if (m_ViewportSize.x != 0 && m_ViewportSize.y != 0)
		{
			m_Camera->Aspect = m_ViewportSize.x / m_ViewportSize.y;
			m_Camera->Planes = { -m_Camera->Aspect, m_Camera->Aspect, -1, 1, m_Camera->Planes.Near, m_Camera->Planes.Far };
		}

		m_Camera->Update();

		m_Framebuffer->Bind();
		RenderCommand::Clear();

		m_Texture->Bind(0);
		m_Texture2->Bind(1);

		if (m_Bricks.size())
			m_Renderer.Submit(m_CubeVertexArray, m_MainShader, m_Bricks.size());
		m_Framebuffer->Unbind();

		RenderCommand::Clear();

		static bool updateFontSize = false;
		static int fontSize = 14;

		if (updateFontSize)
		{
			m_ImGui.GetIO()->Fonts->Clear();
			m_ImGui.GetIO()->Fonts->AddFontFromFileTTF("assets/fonts/e-UkraineHead-Regular.otf", fontSize);
			ImGui_ImplOpenGL3_CreateFontsTexture();
		}

		updateFontSize = false;

		m_ImGui.BeginFrame();
		ImGui::DockSpaceOverViewport();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N")) {}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Add Brick", "Shift+A")) { m_Bricks.push_back(Brick::Create()); m_SortedBricks.push_back(Brick::Create());}
				ImGui::Separator();
				if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
				if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
				if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
				if (ImGui::MenuItem("Delete", "Del")) {}

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

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
		
		for (size_t i = 0; i < m_Bricks.size(); i++)
		{
			auto& brick = m_Bricks[i];
		
			ImGui::PushID(brick->ID);
		
			if (ImGui::TreeNodeEx(brick->Name.c_str(), (m_SelectedBrick == brick ? ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf))
			{
				if (ImGui::IsItemClicked())
				{
					m_SelectedBrick = brick;
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectedBrick)
		{
			ImGui::InputText("Name", &m_SelectedBrick->Name);
			ImGui::DragFloat3("Position", &m_SelectedBrick->Position.x, .1f);
			ImGui::DragFloat3("Rotation", &m_SelectedBrick->Rotation.x, .5f);
			ImGui::DragFloat3("Scale", &m_SelectedBrick->Scale.x, .1f);
			ImGui::ColorEdit4("Color", &m_SelectedBrick->Color.x);
			ImGui::Checkbox("Visible", &m_SelectedBrick->Visible);
		}
		ImGui::End();

		int type = (int)m_Camera->Type;
		int behaviour = (int)m_Camera->Behaviour;

		ImGui::Begin("Debug");

		const float FPS = 1 / m_Timer.Elapsed();
		static float lowestFPS = 1000;
		static float highestFPS = 0;
		static float FPSSum = 0;
		static float FPSCount = 0;
		lowestFPS = (lowestFPS > FPS && FPS > 10 ? FPS : lowestFPS);
		highestFPS = (highestFPS < FPS ? FPS : highestFPS);
		FPSSum += FPS;
		FPSCount++;

		ImGui::Text("FPS: %.1f", FPS);
		ImGui::Text("Average FPS: %.1f", FPSSum / FPSCount);
		ImGui::Text("Min FPS: %.1f", lowestFPS);
		ImGui::Text("Max FPS: %.1f", highestFPS);
		ImGui::Separator();
		ImGui::Text("Map load time: %.3fs", m_LoadTime);
		ImGui::Text("Bricks: %d", m_Bricks.size());
		ImGui::Separator();
		ImGui::Combo("CamType", &type, "Perspective\0Orthographic\0\0");
		ImGui::Combo("CamBehaviour", &behaviour, "None\0Free\0Orbit\0\0");
		ImGui::DragFloat("CamFOV", &m_Camera->FOV, .1f, 0.1f, 120, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
		ImGui::DragFloat("CamZoom", &m_Camera->Zoom, .1f, .1f, 100, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
		ImGui::DragFloat3("CamPos", &m_Camera->Position.x, .1f);
		ImGui::DragFloat3("CamRot", &m_Camera->Rotation.x, .5f);
		ImGui::DragFloat3("CamTar", &m_Camera->TargetPos.x, .1f);

		ImGui::Separator();

		static int fontsize = fontSize;
		ImGui::DragInt("Font Size", &fontsize, .25f, 4, 100, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_Logarithmic);
		if (ImGui::IsItemDeactivated())
		{
			updateFontSize = true;
			fontSize = fontsize;
		}

		m_Camera->Type = (CameraType)type;
		m_Camera->Behaviour = (CameraBehaviour)behaviour;
		ImGui::End();

		m_ImGui.EndFrame();
	}
}