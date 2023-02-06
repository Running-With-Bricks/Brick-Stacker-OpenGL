#pragma once
#include "pch.hpp"

#include <fstream>
#include <filesystem>

#include "ImGui/imgui.h"
#include "imgui_stdlib.h"
#include "ImGui/backends/imgui_impl_glfw.h"
#include "ImGui/backends/imgui_impl_opengl3.h"

#include "BrickStacker/Core/Graphics/Window.hpp"

namespace BrickStacker
{
	class ImGuiLayer
	{
	public:
		ImGuiLayer(const ImGuiLayer&) = delete;
		ImGuiLayer& operator=(const ImGuiLayer&) = delete;

		ImGuiLayer(const Window& window);
		~ImGuiLayer();

		void BeginFrame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void EndFrame()
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}

		ImGuiIO* GetIO()
		{
			return m_IO;
		}

	private:
		void ResetImGuiLayout()
		{
			std::ofstream createImguiIni{ "imgui.ini" };
			createImguiIni <<
R"([Window][Debug##Default]
Pos=0,7
Size=32,32
Collapsed=0

[Window][Dear ImGui Demo]
Collapsed=0

[Window][Viewport]
Pos=0,20
Size=1558,997
Collapsed=0
DockId=0x00000001,0

[Window][Properties]
Pos=1560,604
Size=360,413
Collapsed=0
DockId=0x00000004,0

[Window][Explorer]
Pos=1560,20
Size=222,582
Collapsed=0
DockId=0x00000005,0

[Window][Debug]
Pos=1784,20
Size=136,582
Collapsed=0
DockId=0x00000006,0

[Window][DockSpaceViewport_11111111]
Pos=0,20
Size=1920,997
Collapsed=0

[Docking][Data]
DockSpace       ID=0x8B93E3BD Window=0xA787BDB4 Pos=0,43 Size=1920,997 Split=X Selected=0x13926F0B
  DockNode      ID=0x00000001 Parent=0x8B93E3BD SizeRef=1558,1003 CentralNode=1 Selected=0x13926F0B
  DockNode      ID=0x00000002 Parent=0x8B93E3BD SizeRef=360,1003 Split=Y Selected=0xDA7FC406
    DockNode    ID=0x00000003 Parent=0x00000002 SizeRef=316,586 Split=X Selected=0xDA7FC406
      DockNode  ID=0x00000005 Parent=0x00000003 SizeRef=222,586 Selected=0xDA7FC406
      DockNode  ID=0x00000006 Parent=0x00000003 SizeRef=136,586 Selected=0x392A5ADD
    DockNode    ID=0x00000004 Parent=0x00000002 SizeRef=316,415 Selected=0x199AB496

)";
			ImGui::LoadIniSettingsFromDisk("imgui.ini");
		}

		 ImGuiIO* m_IO;
	};
}