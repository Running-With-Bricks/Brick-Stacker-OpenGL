#pragma once

#include "BrickStacker/Core/Graphics/Window.hpp"

#include "BrickStacker/Core/Renderer/Shader.hpp"
#include "BrickStacker/Core/Renderer/Buffer.hpp"
#include "BrickStacker/Core/Renderer/Renderer.hpp"
#include "BrickStacker/Core/Graphics/FrameBuffer.hpp"

#include "BrickStacker/Base/ImGuiLayer.hpp"
#include "BrickStacker/Base/DiscordRichPresence.hpp"
#include "BrickStacker/Utils/Timer.hpp"

#include <memory>

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

		void Run();

		void Draw();

	private:
		Application();
		~Application();

		Window m_Window{ 800, 600, "Brick Stacker" };

		Ref<Shader> m_MainShader;

		Ref<VertexArray>  m_VertexArray;
		Ref<VertexArray>  m_CubeVertexArray;
		Ref<VertexArray>  m_SkyboxVertexArray;

		Ref<Camera> m_Camera;

		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize{ 0, 0 };

		Timer m_Timer{};
		ImGuiLayer m_ImGui{m_Window};
		Discord& m_Discord = Discord::Get();
		Renderer& m_Renderer = Renderer::Get();
	};
}
