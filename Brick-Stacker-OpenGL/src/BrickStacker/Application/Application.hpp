#pragma once
#include "pch.hpp"

#include "BrickStacker/Core/Graphics/Window.hpp"

#include "BrickStacker/Core/Renderer/Shader.hpp"
#include "BrickStacker/Core/Renderer/Buffer.hpp"
#include "BrickStacker/Core/Renderer/Renderer.hpp"
#include "BrickStacker/Core/Graphics/FrameBuffer.hpp"

#include "BrickStacker/Base/ImGuiLayer.hpp"
#include "BrickStacker/Base/DiscordRichPresence.hpp"

namespace BrickStacker
{
	class Brick
	{
	public:
		static Ref<Brick> Create(const std::string& name = "New Brick")
		{ 
			static uint32_t prevID{ 0 };
			prevID++;
			return CreateRef<Brick>(name, prevID);
		}
		Brick(const std::string& name, uint32_t ID)
			: ID{ ID }
		{
			Name = name;
		}

		const uint32_t ID;
		std::string Name = "Brick";
		glm::vec3 Position{ 0 };
		glm::vec3 Rotation{ 0 };
		glm::vec3 Scale{ 1 };
		glm::vec4 Color{ 1 };
	};



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

		void updateBricksInstancedData();

		Window m_Window{ 800, 600, "Brick Stacker" };

		Ref<Shader> m_MainShader;

		Ref<VertexArray>  m_CubeVertexArray;

		Ref<Camera> m_Camera;

		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize{ 0, 0 };

		Timer m_Timer{};
		ImGuiLayer m_ImGui{m_Window};
		Discord& m_Discord = Discord::Get();
		Renderer& m_Renderer = Renderer::Get();

		std::vector<Ref<Brick>> m_Bricks;
		Ref<Brick> m_SelectedBrick;
	};
}
