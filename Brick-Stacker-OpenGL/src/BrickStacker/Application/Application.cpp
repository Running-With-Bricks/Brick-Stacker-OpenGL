#include "pch.hpp"
#include "Application.hpp"

namespace BrickStacker
{
	Application::Application()
	{
		m_MainShader = Shader::Create("shaders/brick.vert", "shaders/brick.frag");

		m_MainShader->Bind();
		m_MainShader->SetUniformTexture("TopTexture", 0);
		m_MainShader->SetUniformTexture("BottomTexture", 1);
		m_MainShader->Unbind();

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

		m_Scene = SceneSerializer::GetDefaultScene();
		m_Camera = m_Scene->FindEntityByName("Camera");
		RenderCommand::SetClearColor(m_Scene->FindEntityByName("Lighting").GetComponent<LightingComponent>().SkyColor, 1);

		FramebufferSpecifications fbSpecs;
		fbSpecs.Width = 1024;
		fbSpecs.Height = 1024;
		m_Framebuffer = Framebuffer::Create(fbSpecs);

		m_TopBrickTexture = Texture2D::Create("./assets/images/stud_top.png");
		m_BottomBrickTexture = Texture2D::Create("./assets/images/stud_bottom.png");		 
	}

	Application::~Application()
	{
	
	}

	void Application::Run()
	{
		RenderCommand::Clear();

		m_Window.Update();
		m_Window.SetVSync(false);

		m_Discord.SetActivityDetails("Brick Stacking, Brick Build Together");
		m_Discord.SetActivityState("Editing Map.brk");

		Keyboard::Setup();
		Mouse::Setup();

		m_Renderer.SetCamera(m_Scene->GetPrimaryCameraComponent().GetViewMatrix() * m_Scene->GetPrimaryCameraComponent().GetProjectionMatrix());

		while (!m_Window.ShouldClose())
		{
			m_Timer.Reset();

			m_Window.Update();
			m_Discord.Update();

			m_Profiler.AddFrame(1.0f / m_LastFrame);

			Draw();

			m_Timer.Stop();
			m_LastFrame = m_Timer.Elapsed();
		}

		ImGui::SaveIniSettingsToDisk("imgui.ini");
	}

	 void Application::updateBricksInstancedData()
	{
		std::vector<float> instancedData;

		//std::sort(m_SortedBricks.begin(), m_SortedBricks.end(), [m_Scene = m_Scene](const Ref<Brick>& a, const Ref<Brick>& b)
		//	{
		//		if ((a->Color.a == 1.0f) && (b->Color.a != 1.0f)) {
		//			// If a has alpha 1 and b doesn't, a comes first
		//			return true;
		//		}
		//		else if (a->Color.a != 1.0f && b->Color.a == 1.0f) {
		//			// If b has alpha 1 and a doesn't, b comes first
		//			return false;
		//		}
		//		else {
		//
		//			// If both have alpha 1 or both don't have alpha 1, compare by position length
		//			return glm::length(m_Scene->GetPrimaryCameraComponent().Position - a->Position)
		//				 > glm::length(m_Scene->GetPrimaryCameraComponent().Position - b->Position);
		//		}
		//	}
		//);

		auto bricks = m_Scene->GetAllEntitiesWith<BrickComponent>();
	
		for (auto BrickEntityID : bricks)
		{
			Entity BrickEntity{ BrickEntityID, m_Scene.get() };
			if (!BrickEntity.GetComponent<ColorComponent>().Visible)
				continue;

			//pushback brick data into instancedData
			{
				glm::mat4 brickMatrix = BrickEntity.GetComponent<TransformComponent>().GetTransform();

				for (size_t x = 0; x < 4; x++)
				{
					instancedData.push_back(BrickEntity.GetComponent<ColorComponent>().Color[x]);
				}
				instancedData.push_back(BrickEntity.GetComponent<TransformComponent>().Scale.x);
				instancedData.push_back(BrickEntity.GetComponent<TransformComponent>().Scale.z);
				for (size_t x = 0; x < 4; x++)
				{
					for (size_t y = 0; y < 3; y++)
					{
						instancedData.push_back(brickMatrix[x][y]);
					}
				}
			}
		}
		m_CubeVertexArray->GetVertexBuffers()[1]->UpdateBuffer(instancedData, GL_STREAM_DRAW);
	}

	void Application::Draw()
	{
		auto view = m_Scene->GetAllEntitiesWith<NameComponent>();
		auto bricks = m_Scene->GetAllEntitiesWith<BrickComponent>();

		static bool updateFontSize = false;
		static int fontSize = 14;

		if (updateFontSize)
		{
			m_ImGui.GetIO()->Fonts->Clear();
			m_ImGui.GetIO()->Fonts->AddFontFromFileTTF("assets/fonts/e-UkraineHead-Regular.otf", fontSize);
			ImGui_ImplOpenGL3_CreateFontsTexture();
		}

		updateFontSize = false;

		RenderCommand::Clear();

		m_ImGui.BeginFrame();
		ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_::ImGuiDockNodeFlags_PassthruCentralNode);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N")) {}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					if (auto str = FileDialogs::OpenFile("Brick-Hill Map (*.brk)\0*.brk\0"); !str.empty())
					{
						m_Timer.Reset();
						for (auto& entity : m_Scene->GetAllEntitiesWith<BrickComponent>())
							Entity(entity, m_Scene.get()).Destroy();
						m_SelectedEntity = {entt::null, nullptr};

						SceneSerializer::Deserialize(str, m_Scene);
						m_Timer.Stop();
						m_LoadTime = m_Timer.Elapsed();
					};
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					if (auto str = FileDialogs::SaveFile("Brick-Hill Map (*.brk)\0*.brk\0"); !str.empty())
						SceneSerializer::Serialize(str, m_Scene);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Add Brick", "Shift+A")) {}
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
		m_FocusedViewport = ImGui::IsWindowFocused();
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
		
		for (auto& currentEntityIT = view.rbegin(); currentEntityIT != view.rend(); currentEntityIT++)
		{
			Entity currentEntity{ *currentEntityIT, m_Scene.get() };
			ImGui::PushID((int)*currentEntityIT);
		
			auto treeOpened = ImGui::TreeNodeEx("", (m_SelectedEntity == currentEntity ? ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf);
			auto isClicked = ImGui::IsItemClicked();
			ImGui::SameLine(); ImGui::Text(currentEntity.GetName().c_str());
			auto isDoubleClicked = ImGui::IsMouseDoubleClicked(0);
			if (treeOpened)
			{
				if (isClicked)
				{
					m_SelectedEntity = currentEntity;
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::End();
		
		ImGui::Begin("Debug");
		ImGui::Text("FPS: %.1f", m_Profiler.GetFPSBuffer().Back());
		ImGui::Text("Actual FPS: %.1f", m_Profiler.GetFPSBuffer().Current());
		ImGui::Text("Average FPS: %.1f", m_Profiler.GetFPSBuffer().Average());
		ImGui::Text("Max FPS: %.1f", m_Profiler.HighestFPS());
		ImGui::Text("Frame Count: %d", m_Profiler.FrameCount());
		ImGui::Separator();
		ImGui::Text("Map load time: %.3fs", m_LoadTime);
		ImGui::Text("Bricks: %d", bricks.size());

		ImGui::Separator();

		static int fontsize = fontSize;
		ImGui::DragInt("Font Size", &fontsize, .25f, 4, 100, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_Logarithmic);
		if (ImGui::IsItemDeactivated())
		{
			updateFontSize = true;
			fontSize = fontsize;
		}

		ImGui::End();

		ImGui::Begin("Properties");

		ImGui::PushID((int)m_SelectedEntity);
		if (m_SelectedEntity)
		{
			if (m_SelectedEntity.HasComponent<NameComponent>())
			{
				if (ImGui::TreeNodeEx("Data", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
				{
					//ImGui::LabelText("##n", "Name"); ImGui::SameLine(NULL, 1);
					ImGui::InputText("Name", &m_SelectedEntity.GetComponent<NameComponent>().Name);

					ImGui::TreePop();
				}
				ImGui::Separator();
			}
			if (m_SelectedEntity.HasComponent<BrickComponent>())
			{
				/// Disabled cuz no other shapes other than Cube so yeah
				//if (ImGui::TreeNodeEx("Brick", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
				//{
				//	
				//}
			}
			if (m_SelectedEntity.HasComponent<BaseplateComponent>())
			{
				if (ImGui::TreeNodeEx("Baseplate", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::DragUint("Size", &m_SelectedEntity.GetComponent<BaseplateComponent>().Size, 0.1f, 0, UINT32_MAX, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
					ImGui::TreePop();
				}
				ImGui::Separator();
			}
			if (m_SelectedEntity.HasComponent<CameraComponent>())
			{
				int type = (int)m_SelectedEntity.GetComponent<CameraComponent>().camera.Type;
				int behaviour = (int)m_SelectedEntity.GetComponent<CameraComponent>().camera.Behaviour;

				if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Combo("Type", &type, "Perspective\0Orthographic\0\0");
					ImGui::Combo("Behaviour", &behaviour, "None\0Free\0Orbit\0\0");

					if ((CameraType)type == CameraType::Perspective)
						ImGui::DragFloat("FOV", &m_SelectedEntity.GetComponent<CameraComponent>().camera.FOV, .1f, 0.1f, 179, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
					else if ((CameraType)type == CameraType::Orthographic)
						ImGui::DragFloat("Zoom", &m_SelectedEntity.GetComponent<CameraComponent>().camera.Zoom, .1f, .1f, 100, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);

					if ((CameraBehaviour)behaviour == CameraBehaviour::Free)
					{
						if (ImGui::TreeNodeEx("Movement", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanFullWidth))
						{
							ImGui::DragFloat("Start Accelerating After", &m_CameraController.CameraSettings.AccelerateAfter, .1f, 0.f, 100, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
							ImGui::DragFloat("Acceleration", &m_CameraController.CameraSettings.Acceleration, .1f, 0.f, 100, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
							ImGui::DragFloat("Speed", &m_CameraController.CameraSettings.Speed, .5f, .1f, 100, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
							ImGui::DragFloat("Shift Multiplier", &m_CameraController.CameraSettings.ShiftMultiplier, .25f, .1f, 100, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
							ImGui::Checkbox("Ignore Delta Time", &m_CameraController.CameraSettings.IgnoreDeltaTime);
							ImGui::TreePop();
						}
					}

					if ((CameraBehaviour)behaviour != CameraBehaviour::None)
					{
						if (ImGui::TreeNodeEx("Turning", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanFullWidth))
						{
							if ((CameraBehaviour)behaviour == CameraBehaviour::Orbit)
								ImGui::DragFloat("Distance", &m_SelectedEntity.GetComponent<CameraComponent>().camera.Distance, .5f, 0.0125f, 10000, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
							ImGui::DragFloat("Sensitivity", &m_CameraController.CameraSettings.Sensitivity, .05f, 0.0125f, 100, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
							ImGui::Checkbox("Invert X", &m_CameraController.CameraSettings.InvertX);
							ImGui::Checkbox("Invert Y", &m_CameraController.CameraSettings.InvertY);
							if ((CameraBehaviour)behaviour == CameraBehaviour::Orbit)
								ImGui::Checkbox("Invert Scroll", &m_CameraController.CameraSettings.InvertScroll);
							ImGui::TreePop();
						}
					}

					ImGui::TreePop();
				}
				ImGui::Separator();

				if ((CameraBehaviour)behaviour != CameraBehaviour::None)
				{
					if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
					{
						if ((CameraBehaviour)behaviour == CameraBehaviour::Free)
						{
							ImGui::DragFloat3("Position", &m_SelectedEntity.GetComponent<CameraComponent>().camera.Position.x, .1f);
							ImGui::DragFloat3("Rotation", &m_SelectedEntity.GetComponent<CameraComponent>().camera.Rotation.x, .5f);
						}
						else if ((CameraBehaviour)behaviour == CameraBehaviour::Orbit)
							ImGui::DragFloat3("Target Position", &m_SelectedEntity.GetComponent<CameraComponent>().camera.TargetPos.x, .1f);

						ImGui::TreePop();
					}
					ImGui::Separator();
				}

				m_SelectedEntity.GetComponent<CameraComponent>().camera.Type = (CameraType)type;
				m_SelectedEntity.GetComponent<CameraComponent>().camera.Behaviour = (CameraBehaviour)behaviour;
			}
			if (m_SelectedEntity.HasComponent<LightingComponent>())
			{
				if (ImGui::TreeNodeEx("Lighting", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
				{
					auto prevSkyColor = m_SelectedEntity.GetComponent<LightingComponent>().SkyColor;

					ImGui::ColorEdit3("Sky Color", &m_SelectedEntity.GetComponent<LightingComponent>().SkyColor.x);
					ImGui::ColorEdit3("Ambient Color", &m_SelectedEntity.GetComponent<LightingComponent>().AmbientColor.x);
					ImGui::DragUint("Sun Intensity", &m_SelectedEntity.GetComponent<LightingComponent>().SunIntensity, 0.1f, 0, UINT32_MAX, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
					ImGui::TreePop();

					if (prevSkyColor != m_SelectedEntity.GetComponent<LightingComponent>().SkyColor)
						RenderCommand::SetClearColor(m_SelectedEntity.GetComponent<LightingComponent>().SkyColor, 1);
				}
				ImGui::Separator();
			}

			if (m_SelectedEntity.HasComponent<TransformComponent>())
			{
				if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::DragFloat3("Position", &m_SelectedEntity.GetComponent<TransformComponent>().Position.x, .1f);
					ImGui::DragInt("Rotation", &m_SelectedEntity.GetComponent<TransformComponent>().Rotation, .5f);
					ImGui::DragFloat3("Scale", &m_SelectedEntity.GetComponent<TransformComponent>().Scale.x, .1f);

					ImGui::TreePop();
				}
				ImGui::Separator();
			}
			if (m_SelectedEntity.HasComponent<ColorComponent>())
			{
				if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::ColorEdit4("Color", &m_SelectedEntity.GetComponent<ColorComponent>().Color.x);
					ImGui::Checkbox("Visible", &m_SelectedEntity.GetComponent<ColorComponent>().Visible);

					ImGui::TreePop();
				}
				ImGui::Separator();
			}
		}
		ImGui::PopID();
		ImGui::End();

		m_ImGui.EndFrame();



		updateBricksInstancedData();

		if (m_ViewportSize.x != 0 && m_ViewportSize.y != 0)
		{
			m_Scene->GetPrimaryCameraComponent().Aspect = m_ViewportSize.x / m_ViewportSize.y;
			m_Scene->GetPrimaryCameraComponent().Planes = { -m_Scene->GetPrimaryCameraComponent().Aspect, m_Scene->GetPrimaryCameraComponent().Aspect, -1, 1, m_Scene->GetPrimaryCameraComponent().Planes.Near, m_Scene->GetPrimaryCameraComponent().Planes.Far };
		}

		m_CameraController.Update(m_LastFrame);
		m_Renderer.SetCamera(m_Scene->GetPrimaryCameraComponent().GetProjectionMatrix() * m_Scene->GetPrimaryCameraComponent().GetViewMatrix());

		m_Framebuffer->Bind();
		RenderCommand::Clear();

		m_TopBrickTexture->Bind(0);
		m_BottomBrickTexture->Bind(1);

		if (auto size = bricks.size(); size)
		{
			m_Renderer.Submit(m_CubeVertexArray, m_MainShader, size);
		}

		m_Framebuffer->Unbind();
	}
}