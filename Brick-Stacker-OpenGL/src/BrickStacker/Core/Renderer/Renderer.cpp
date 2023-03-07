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
		shader->Bind();

		shader->SetUniformMat4("u_ViewMatrix", m_SceneCamera->GetViewMatrix());
		shader->SetUniformMat4("u_ProjectionMatrix", m_SceneCamera->GetProjectionMatrix());

		vertexArray->Bind();
		RenderCommand::DrawIndexedInstanced(vertexArray, count);


		shader->Unbind();
		vertexArray->Unbind();
	}
}
