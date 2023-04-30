#include "pch.hpp"
#include "Renderer.hpp"

namespace BrickStacker
{
	void Renderer::SetCamera(const Ref<Camera>& camera)
	{
		m_SceneCamera = camera;
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, int count)
	{
		auto sceneCamera = m_SceneCamera.lock();
		if (!sceneCamera)
			return;

		shader->Bind();

		shader->SetUniformMat4("u_ViewMatrix", sceneCamera->GetViewMatrix());
		shader->SetUniformMat4("u_ProjectionMatrix", sceneCamera->GetProjectionMatrix());

		vertexArray->Bind();
		RenderCommand::DrawIndexedInstanced(vertexArray, count);

		shader->Unbind();
		vertexArray->Unbind();
	}
}
