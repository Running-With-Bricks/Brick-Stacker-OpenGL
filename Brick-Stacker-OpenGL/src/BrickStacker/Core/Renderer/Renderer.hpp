#pragma once
#include "pch.hpp"

#include "RenderCommand.hpp"
#include "BrickStacker/Core/Renderer/Shader.hpp"
#include "BrickStacker/Core/Graphics/Texture.hpp"
#include "BrickStacker/Core/Scene/Camera.hpp"

namespace BrickStacker
{
	class Renderer
	{
	public:
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		static Renderer& Get()
		{
			static Renderer renderer{};
			return renderer;
		}

		void SetCamera(const glm::mat4& viewProjectionMatrix);

		void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, int count);
	private:
		Renderer()
		{
			glEnable(GL_DEPTH_TEST);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CCW);
		}
		~Renderer() {}

		glm::mat4 m_CameraViewProjection;
	};
}
