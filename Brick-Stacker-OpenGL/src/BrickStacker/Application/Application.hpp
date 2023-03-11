#pragma once
#include "pch.hpp"

#include "BrickStacker/Core/Graphics/Window.hpp"

#include "BrickStacker/Core/Renderer/Shader.hpp"
#include "BrickStacker/Core/Graphics/Texture.hpp"
#include "BrickStacker/Core/Renderer/Buffer.hpp"
#include "BrickStacker/Core/Renderer/Renderer.hpp"
#include "BrickStacker/Core/Graphics/FrameBuffer.hpp"

#include "BrickStacker/Base/ImGuiLayer.hpp"
#include "BrickStacker/Base/DiscordRichPresence.hpp"
#include "BrickStacker/Utils/Profiler.hpp"

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
		bool Visible = true;
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

		const Window& GetWindow() const { return m_Window; };

		void Run();

		void Draw();

	private:
		Application();
		~Application();

		void updateBricksInstancedData();

		Window m_Window{ 800, 600, "Brick Stacker" };

		Ref<Shader> m_MainShader;
		Ref<Shader> m_TestShader;

		Ref<Texture2D> m_TopBrickTexture;
		Ref<Texture2D> m_BottomBrickTexture;

		Ref<VertexArray> m_CubeVertexArray;

		Ref<Camera> m_Camera;

		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize{ 1920, 1080 };

		float m_LoadTime;

		Timer m_Timer{};
		ImGuiLayer m_ImGui{m_Window};
		Discord& m_Discord = Discord::Get();
		Renderer& m_Renderer = Renderer::Get();
		Profiler& m_Profiler = Profiler::Get();

		std::vector<Ref<Brick>> m_Bricks;
		std::vector<Ref<Brick>> m_SortedBricks;
		Ref<Brick> m_SelectedBrick;
	};
}
