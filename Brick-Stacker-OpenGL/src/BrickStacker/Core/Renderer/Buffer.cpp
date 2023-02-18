#include "pch.hpp"
#include "Buffer.hpp"

namespace BrickStacker
{
	//-------------//
	//Vertex Buffer//
	//-------------//

	VertexBuffer::VertexBuffer(const std::vector<float>& verticies, GLenum usage)
	{
		glGenBuffers(1, &m_RendererID);
		UpdateBuffer(verticies, usage);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::UpdateBuffer(const std::vector<float>& verticies, GLenum usage)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//------------//
	//Index Buffer//
	//------------//

	IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indicies)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		m_Count = (uint32_t)indicies.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), indicies.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	uint32_t IndexBuffer::GetCount() const
	{
		return m_Count;
	}

	//------------//
	//Vertex Array//
	//------------//

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		BS_ASSERT(vertexBuffer->GetLayout().GetElements().size() > 0, "Provided VertexBuffer has no BufferLayout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		for (const auto& buffer : m_VertexBuffers)
		{
			index += (uint32_t)buffer->GetLayout().GetElements().size();
		}

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), element.GetOpenGLType(), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(element.Offset));

			if (element.Instanced)
				glVertexAttribDivisor(index, 1);

			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);

		glBindVertexArray(0);
		vertexBuffer->Unbind();
	}

	void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;

		glBindVertexArray(0);
		indexBuffer->Unbind();
	}
}
