#include "pch.hpp"
#include "Renderer.hpp"

namespace BrickStacker
{
	void Renderer::SetCamera(const glm::mat4& viewProjectionMatrix)
	{
		m_CameraViewProjection = viewProjectionMatrix;
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, int count)
	{
		shader->Bind();

		shader->SetUniformMat4("u_ViewProjectionMatrix", m_CameraViewProjection);

		vertexArray->Bind();
		RenderCommand::DrawIndexedInstanced(vertexArray, count);

		shader->Unbind();
		vertexArray->Unbind();
	}
}
