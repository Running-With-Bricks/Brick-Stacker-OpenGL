#include "ImGuiLayer.hpp"

#include <fstream>

namespace BrickStacker
{
	ImGuiLayer::ImGuiLayer(const Window& window)
	{
        

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        m_IO = &ImGui::GetIO(); (void)m_IO;
        (*m_IO).ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        (*m_IO).ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        (*m_IO).ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        m_IO->Fonts->AddFontFromFileTTF("assets/fonts/e-UkraineHead-Regular.otf", 14);

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if ((*m_IO).ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 460");

        std::ifstream imguiIni{ "imgui.ini" };
        if (!imguiIni.is_open())
            ImGui::LoadIniSettingsFromDisk("default.ini");
	}

	ImGuiLayer::~ImGuiLayer()
	{

	}
}
