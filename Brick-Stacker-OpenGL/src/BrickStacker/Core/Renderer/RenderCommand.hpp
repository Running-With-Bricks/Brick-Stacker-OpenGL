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

		inline static void SetClearColor(const glm::vec3& color, float alpha)
		{
			glClearColor(color.r, color.g, color.b, alpha);
		}

		inline static void Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			IfLines(vertexArray);
			glDrawElements((GLenum)vertexArray->renderingMode, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}

		inline static void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, int count)
		{
			IfLines(vertexArray);
			glDrawElementsInstanced((GLenum)vertexArray->renderingMode, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, count);
		}
	private:
		inline static void IfLines(const Ref<VertexArray>& vertexArray)
		{
			if (vertexArray->renderingMode == RenderingMode::Lines)
				glLineWidth(vertexArray->lineWidth);
		}
	};
}
