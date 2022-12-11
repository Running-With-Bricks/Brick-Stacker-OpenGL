#include "Renderer.hpp"

namespace BrickStacker
{
	void Renderer::SetCamera(const std::shared_ptr<Camera>& camera)
	{
		m_SceneCamera = camera;
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::vec3& scale)
	{
		shader->Bind();

		shader->SetUniformMat4("u_Transform", glm::scale(glm::mat4{ 1 }, scale));
		shader->SetUniformMat4("u_ViewMatrix", m_SceneCamera->GetViewMatrix());
		shader->SetUniformMat4("u_ProjectionMatrix", m_SceneCamera->GetProjectionMatrix());

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);

		shader->Unbind();
		vertexArray->Unbind();
	}
}
