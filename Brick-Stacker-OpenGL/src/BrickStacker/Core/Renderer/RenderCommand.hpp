#pragma once
#include "pch.hpp"

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

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}

		inline static void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, int count)
		{
			glDrawElementsInstanced(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, count);
		}
	};
}
