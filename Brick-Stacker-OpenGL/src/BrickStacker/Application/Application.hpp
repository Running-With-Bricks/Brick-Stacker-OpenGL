#pragma once

#include "BrickStacker/Core/Graphics/Window.hpp"

#include "BrickStacker/Core/Renderer/Shader.hpp"
#include "BrickStacker/Core/Renderer/Buffer.hpp"

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
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<VertexBuffer> m_PyramidVertexBuffer;
		std::shared_ptr<IndexBuffer>  m_PyramidIndexBuffer;
		std::shared_ptr<VertexArray>  m_PyramidVertexArray;

		Timer m_Timer{};

		Discord& m_Discord = Discord::Get();
	};
}
