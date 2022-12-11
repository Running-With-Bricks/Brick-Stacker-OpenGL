#pragma once

#include "BrickStacker/Core/Graphics/Window.hpp"

#include "BrickStacker/Core/Renderer/Shader.hpp"
#include "BrickStacker/Core/Renderer/Buffer.hpp"
#include "BrickStacker/Core/Renderer/Renderer.hpp"

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

		std::shared_ptr<Shader> m_MainShader;

		std::shared_ptr<VertexArray>  m_VertexArray;
		std::shared_ptr<VertexArray>  m_CubeVertexArray;
		std::shared_ptr<VertexArray>  m_SkyboxVertexArray;

		std::shared_ptr<Camera> m_Camera;

		Timer m_Timer{};
		ImGuiLayer m_ImGui{m_Window};
		Discord& m_Discord = Discord::Get();
		Renderer& m_Renderer = Renderer::Get();
	};
}
