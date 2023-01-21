#pragma once

#include "BrickStacker/Core/Core.hpp"

#include "RenderCommand.hpp"
#include "BrickStacker/Core/Renderer/Shader.hpp"
#include "BrickStacker/Core/Scene/Camera.hpp"

#include <memory>
#include <vector>

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

		void SetCamera(const Ref<Camera>& camera);

		void EndScene();

		void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::vec3& scale = { 1, 1, 1 });
	private:
		Renderer()
		{
			glEnable(GL_DEPTH_TEST);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CCW);
		}
		~Renderer() {}

		Ref<Camera> m_SceneCamera;
	};
}
