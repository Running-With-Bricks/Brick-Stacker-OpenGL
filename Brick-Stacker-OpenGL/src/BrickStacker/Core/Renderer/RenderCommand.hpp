#pragma once

#include "glad/glad.h"

#include "glm/glm.hpp"

#include "Buffer.hpp"

namespace BrickStacker
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			glClearColor(color.r, color.g, color.b, color.a);
		}

		inline static void Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	};
}
