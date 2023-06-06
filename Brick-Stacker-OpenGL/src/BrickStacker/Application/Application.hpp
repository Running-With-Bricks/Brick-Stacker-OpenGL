#pragma once
#include "pch.hpp"

#include "glm/glm.hpp"

#include "BrickStacker/Core/Graphics/Window.hpp"

#include "BrickStacker/Core/Renderer/Shader.hpp"
#include "BrickStacker/Core/Graphics/Texture.hpp"
#include "BrickStacker/Core/Renderer/Buffer.hpp"
#include "BrickStacker/Core/Renderer/Renderer.hpp"
#include "BrickStacker/Core/Graphics/FrameBuffer.hpp"
#include "BrickStacker/Core/Input/Keyboard.hpp"
#include "BrickStacker/Core/Input/Mouse.hpp"
#include "BrickStacker/Core/Scene/Scene.hpp"
#include "BrickStacker/Core/Scene/Entity.hpp"
#include "BrickStacker/Core/Scene/SceneSerializer.hpp"
#include "BrickStacker/Core/Scene/CameraController.hpp"

#include "BrickStacker/Base/ImGuiLayer.hpp"
#include "BrickStacker/Base/DiscordRichPresence.hpp"
#include "BrickStacker/Utils/Profiler.hpp"
#include "BrickStacker/Utils/PlatformUtils.hpp"
#include "BrickStacker/Base/AssetManager.hpp"

namespace BrickStacker
{
	class Application
	{
	public:
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		static Application& Get()
		{
			static Application Application{};
			return Application;
		}

		const Window& GetWindow() const { return m_Window; };

		void Run();

		void Draw();

		Ref<Scene> GetFocusedScene() { return m_FocusedViewport ? m_Scene : nullptr; };
		std::vector<Ref<Scene>> GetViewports() { return { m_Scene }; };

	private:
		Application();
		~Application() = default;

		Window m_Window{ 1080, 608, "Brick Stacker" };

		Ref<Texture2D> m_TopBrickTexture;
		Ref<Texture2D> m_BottomBrickTexture;

		Ref<Scene> m_Scene;
		CameraController& m_CameraController = CameraController::Get();

		Ref<Framebuffer> m_Framebuffer;
		ImVec2 m_ViewportSize{ 1920, 1080 };

		float m_LoadTime;
		bool m_FocusedViewport = false;

		Timer m_FrameTimer{};
		Timer m_DrawTimer{};
		float m_LastFrame{ 1 };
		ImGuiLayer m_ImGui{m_Window};
		Discord& m_Discord = Discord::Get();
		Profiler& m_GlobalProfiler = Profiler::Get();
		Profiler& m_DrawProfiler = Profiler();

		Entity m_SelectedEntity;
	};
}
