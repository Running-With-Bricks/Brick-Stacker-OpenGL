#include "pch.hpp"
#include "Application.hpp"

namespace BrickStacker
{
	Application::Application()
	{
		m_Scene = SceneSerializer::GetDefaultScene();
		RenderCommand::SetClearColor(m_Scene->FindEntityByName("Lighting").GetComponent<LightingComponent>().SkyColor, 1);

		FramebufferSpecifications fbSpecs;
		fbSpecs.Width = 1024;
		fbSpecs.Height = 1024;
		m_Framebuffer = Framebuffer::Create(fbSpecs);

		m_TopBrickTexture = Texture2D::Create("./assets/images/stud_top.png");
		m_BottomBrickTexture = Texture2D::Create("./assets/images/stud_bottom.png");
	}

	void Application::Run()
	{
		RenderCommand::Clear();

		m_Window.Update();
		m_Window.SetVSync(true);

		m_Discord.SetActivityDetails("Brick Stacking, Brick Build Together");
		m_Discord.SetActivityState("Editing Map.brk");

		Keyboard::Setup();
		Mouse::Setup();

		while (!m_Window.ShouldClose())
		{
			m_FrameTimer.Reset();

			m_Window.Update();
			m_Discord.Update();
			m_CameraController.Update(m_LastFrame);

			m_DrawTimer.Reset();
			Draw();
			m_DrawTimer.Stop();

			m_FrameTimer.Stop();
			m_LastFrame = m_FrameTimer.Elapsed();
			m_GlobalProfiler.AddFrame(m_LastFrame);
			m_DrawProfiler.AddFrame(m_DrawTimer.Elapsed());
		}

		ImGui::SaveIniSettingsToDisk("imgui.ini");
	}

	void Application::Draw()
	{
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
				if (ImGui::MenuItem("New", "Ctrl+N") && FileDialogs::ChangeFile(m_Scene))
				{
					m_Scene = SceneSerializer::GetDefaultScene();
					m_SelectedEntity = Entity();
				}
				if (ImGui::MenuItem("Open", "Ctrl+O") && FileDialogs::ChangeFile(m_Scene))
				{
					if (auto str = FileDialogs::OpenFile("Brick-Hill Map (*.brk)\0*.brk\0"); !str.empty())
					{
						m_FrameTimer.Reset();
						m_SelectedEntity = Entity();
						m_Scene = SceneSerializer::GetDefaultScene();
						m_Scene->FilePath = str;
						auto DefCam = m_Scene->GetPrimaryCameraEntity();

						SceneSerializer::Deserialize({ str, m_Scene });
						if (DefCam != m_Scene->GetPrimaryCameraEntity())
						{
							DefCam.Destroy();
						}
						m_FrameTimer.Stop();
						m_LoadTime = m_FrameTimer.Elapsed();						
					};
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					if (auto str = !m_Scene->FilePath.empty() ? m_Scene->FilePath : FileDialogs::SaveFile("Brick-Hill Map (*.brk)\0*.brk\0"); !str.empty())
					{
						SceneSerializer::Serialize({ m_Scene->FilePath, m_Scene });
						m_Scene->FilePath = str;
					}
				}
				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
				{
					if (auto str = FileDialogs::SaveFile("Brick-Hill Map (*.brk)\0*.brk\0"); !str.empty())
					{
						SceneSerializer::Serialize({ str, m_Scene });
						m_Scene->FilePath = str;
					}
				}
				bool a = false;
				ImGui::Checkbox("Include Brick-Stacker Specifics", &a);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Add Brick", "Ctrl+A"))
				{
					auto b = m_Scene->CreateBrick();
					b.GetComponent<TransformComponent>().Position = m_Scene->Raycast(m_Scene->GetPrimaryCameraComponent(), { 0, 0 }).first;
					b.GetComponent<PhysicsComponent>().Update();
				}
				if (ImGui::MenuItem("Add Camera", NULL))
					m_Scene->CreateCamera();
				ImGui::Separator();
				if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
				if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
				if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
				if (ImGui::MenuItem("Delete", "Del", false, (m_SelectedEntity && m_SelectedEntity != m_Scene->GetPrimaryCameraEntity() && !m_SelectedEntity.HasComponent<LightingComponent>() && !m_SelectedEntity.HasComponent<BaseplateComponent>())))
				{
					m_SelectedEntity.Destroy();
					m_SelectedEntity = Entity();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		auto view = m_Scene->GetAllEntitiesWith<NameComponent>();
		auto bricks = m_Scene->GetAllEntitiesWith<BrickComponent>();

		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		m_FocusedViewport = ImGui::IsWindowFocused();
		auto viewportSize = ImGui::GetContentRegionAvail();
		auto isMouseClicked = ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left);
		if (m_ViewportSize != viewportSize && viewportSize.x != 0 && viewportSize.y != 0)
		{
			m_ViewportSize = { viewportSize.x, viewportSize.y };
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
		if (isMouseClicked)
		{
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportOffset = ImGui::GetWindowPos();
			auto pos = ImGui::GetMousePos();
			auto windowPos = ImGui::GetWindowContentRegionMin();
			glm::vec2 mappedPos = { (((pos.x - (viewportMinRegion.x + viewportOffset.x)) / viewportSize.x) - 0.5f) * 2, (((pos.y - (viewportMinRegion.y + viewportOffset.y)) / viewportSize.y) - 0.5f) * 2 };
			auto rres = m_Scene->Raycast(m_Scene->GetPrimaryCameraComponent(), mappedPos);
			m_SelectedEntity = { rres.second, m_Scene.get()};
		}

		auto textureID = m_Framebuffer->GetColorAttachmentRendererID();

		if (textureID)
			ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Explorer");
		
		for (auto& entityID : m_Scene->GetRoot())
		{
			Entity currentEntity{ entityID, m_Scene.get() };
			ImGui::PushID((int)entityID);
		
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
		ImGui::Text("Frame Count: %d", m_GlobalProfiler.FrameCount());
		if (ImGui::TreeNodeEx("Frame time", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Average FPS: %.3f", 1 / (m_GlobalProfiler.GetFPSBuffer().Average() * 0.001f));
			ImGui::Text("Actual ms: %.3fms", m_GlobalProfiler.GetFPSBuffer().Current());
			ImGui::Text("Average ms: %.3fms", m_GlobalProfiler.GetFPSBuffer().Average());
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Draw time", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Average FPS: %.3f", 1 / (m_DrawProfiler.GetFPSBuffer().Average() * 0.001f));
			ImGui::Text("Actual ms: %.3fms", m_DrawProfiler.GetFPSBuffer().Current());
			ImGui::Text("Average ms: %.3fms", m_DrawProfiler.GetFPSBuffer().Average());
			ImGui::TreePop();
		}
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

				if (ImGui::TreeNodeEx("Data", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
				{
					//ImGui::LabelText("##n", "Name"); ImGui::SameLine(NULL, 1);
				if (m_SelectedEntity.HasComponent<NameComponent>())
					ImGui::InputText("Name", &m_SelectedEntity.GetComponent<NameComponent>().Name);

				//Shit ass code
				if (m_SelectedEntity.HasComponent<BrickComponent>())
					ImGui::LabelText("Class", "Brick");
				else if (m_SelectedEntity.HasComponent<BaseplateComponent>())
					ImGui::LabelText("Class", "Baseplate");
				else if (m_SelectedEntity.HasComponent<CameraComponent>())
					ImGui::LabelText("Class", "Camera");
				else if (m_SelectedEntity.HasComponent<LightingComponent>())
					ImGui::LabelText("Class", "Lighting");
				else
					ImGui::LabelText("Class", "Undefined");

					ImGui::TreePop();
				}
				ImGui::Separator();

			/// Disabled cuz no other shapes other than Cube so yeah
			//if (m_SelectedEntity.HasComponent<BrickComponent>())
			//{
			//	if (ImGui::TreeNodeEx("Brick", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
			//	{
			//	
			//	}
			//}
			if (m_SelectedEntity.HasComponent<BaseplateComponent>())
			{
				if (ImGui::TreeNodeEx("Baseplate", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::DragUint("Size", &m_SelectedEntity.GetComponent<BaseplateComponent>().Size, 0.1f, 0, UINT32_MAX, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp);
					ImGui::EntityEdited(m_SelectedEntity);
					ImGui::TreePop();
				}
				ImGui::Separator();
				if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::ColorEdit3("Color", &m_SelectedEntity.GetComponent<BaseplateComponent>().Color.x);
					ImGui::Checkbox("Visible", &m_SelectedEntity.GetComponent<BaseplateComponent>().Visible);

					ImGui::TreePop();
				}
				ImGui::Separator();
			}
			else if (m_SelectedEntity.HasComponent<CameraComponent>())
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
						ImGui::DragFloat("Zoom", &m_SelectedEntity.GetComponent<CameraComponent>().camera.Zoom, .1f, 0.f, 1000, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_::ImGuiSliderFlags_Logarithmic);

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
					if (ImGui::TreeNodeEx("Planes", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanFullWidth))
					{
						ImGui::DragFloat("Near", &m_SelectedEntity.GetComponent<CameraComponent>().camera.Planes.Near, 0.1f, 0.01f, m_SelectedEntity.GetComponent<CameraComponent>().camera.Planes.Far, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_::ImGuiSliderFlags_Logarithmic);
						ImGui::DragFloat("Far", &m_SelectedEntity.GetComponent<CameraComponent>().camera.Planes.Far, 0.1f, m_SelectedEntity.GetComponent<CameraComponent>().camera.Planes.Near, 10000, NULL, ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_::ImGuiSliderFlags_Logarithmic);
						ImGui::TreePop();
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

				if (m_Scene->GetPrimaryCameraEntity() != m_SelectedEntity)
				{
					if (ImGui::Button("Set as Primary Camera"))
						m_Scene->SetPrimaryCameraEntity(m_SelectedEntity);
				}
				else
					ImGui::Text("This is a Primary Camera");

				m_SelectedEntity.GetComponent<CameraComponent>().camera.Type = (CameraType)type;
				m_SelectedEntity.GetComponent<CameraComponent>().camera.Behaviour = (CameraBehaviour)behaviour;
			}
			else if (m_SelectedEntity.HasComponent<LightingComponent>())
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
					ImGui::DragFloat3("Position", &m_SelectedEntity.GetComponent<TransformComponent>().Position.x, .1f); ImGui::EntityEdited(m_SelectedEntity);
					ImGui::DragInt("Rotation", &m_SelectedEntity.GetComponent<TransformComponent>().Rotation, .5f); ImGui::EntityEdited(m_SelectedEntity);
					ImGui::DragFloat3("Scale", &m_SelectedEntity.GetComponent<TransformComponent>().Scale.x, .1f); ImGui::EntityEdited(m_SelectedEntity);

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


		if (m_ViewportSize.x != 0 && m_ViewportSize.y != 0)
		{
			m_Scene->GetPrimaryCameraComponent().Aspect = m_ViewportSize.x / m_ViewportSize.y;
			m_Scene->GetPrimaryCameraComponent().Planes = { -m_Scene->GetPrimaryCameraComponent().Aspect, m_Scene->GetPrimaryCameraComponent().Aspect, -1, 1, m_Scene->GetPrimaryCameraComponent().Planes.Near, m_Scene->GetPrimaryCameraComponent().Planes.Far };
		}

		m_Framebuffer->Bind();
		RenderCommand::Clear();

		m_TopBrickTexture->Bind(0);
		m_BottomBrickTexture->Bind(1);

		m_Scene->RenderScene();

		m_Framebuffer->Unbind();
	}
}