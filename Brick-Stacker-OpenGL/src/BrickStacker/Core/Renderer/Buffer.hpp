#pragma once
#include "pch.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace BrickStacker
{
	enum class ShaderDataType
	{
		None = 0,
		Float, Vec2, Vec3, Vec4,
		Int, IVec2, IVec3, IVec4,
		Uint, UVec2, UVec3, UVec4,
		Bool, BVec2, BVec3, BVec4,
		Double, DVec2, DVec3, DVec4,
		Mat2, Mat3, Mat4
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:  return 4;
			case ShaderDataType::Vec2:   return 4 * 2;
			case ShaderDataType::Vec3:   return 4 * 3;
			case ShaderDataType::Vec4:   return 4 * 4;
			case ShaderDataType::Int:    return 4;
			case ShaderDataType::IVec2:  return 4 * 2;
			case ShaderDataType::IVec3:  return 4 * 3;
			case ShaderDataType::IVec4:  return 4 * 4;
			case ShaderDataType::Uint:   return 4;
			case ShaderDataType::UVec2:  return 4 * 2;
			case ShaderDataType::UVec3:  return 4 * 3;
			case ShaderDataType::UVec4:  return 4 * 4;
			case ShaderDataType::Bool:   return 1;
			case ShaderDataType::BVec2:  return 1 * 2;
			case ShaderDataType::BVec3:  return 1 * 3;
			case ShaderDataType::BVec4:  return 1 * 4;
			case ShaderDataType::Double: return 8;
			case ShaderDataType::DVec2:  return 8 * 2;
			case ShaderDataType::DVec3:  return 8 * 3;
			case ShaderDataType::DVec4:  return 8 * 4;
			case ShaderDataType::Mat2:   return 4 * 2 * 2;
			case ShaderDataType::Mat3:   return 4 * 3 * 3;
			case ShaderDataType::Mat4:   return 4 * 4 * 4;
		}

		BS_ASSERT(false, "Unknown ShaderDataType: {0}", (int)type);
	}

	static uint32_t GetShaderDataTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
			case BrickStacker::ShaderDataType::Float:	return 1;
			case BrickStacker::ShaderDataType::Vec2:	return 2;
			case BrickStacker::ShaderDataType::Vec3:	return 3;
			case BrickStacker::ShaderDataType::Vec4:	return 4;
			case BrickStacker::ShaderDataType::Int:		return 1;
			case BrickStacker::ShaderDataType::IVec2:	return 2;
			case BrickStacker::ShaderDataType::IVec3:	return 3;
			case BrickStacker::ShaderDataType::IVec4:	return 4;
			case BrickStacker::ShaderDataType::Uint:	return 1;
			case BrickStacker::ShaderDataType::UVec2:	return 2;
			case BrickStacker::ShaderDataType::UVec3:	return 3;
			case BrickStacker::ShaderDataType::UVec4:	return 4;
			case BrickStacker::ShaderDataType::Bool:	return 1;
			case BrickStacker::ShaderDataType::BVec2:	return 2;
			case BrickStacker::ShaderDataType::BVec3:	return 3;
			case BrickStacker::ShaderDataType::BVec4:	return 4;
			case BrickStacker::ShaderDataType::Double:	return 1;
			case BrickStacker::ShaderDataType::DVec2:	return 2;
			case BrickStacker::ShaderDataType::DVec3:	return 3;
			case BrickStacker::ShaderDataType::DVec4:	return 4;
			case BrickStacker::ShaderDataType::Mat2:	return 2 * 2;
			case BrickStacker::ShaderDataType::Mat3:	return 3 * 3;
			case BrickStacker::ShaderDataType::Mat4:	return 4 * 4;
		}

		BS_ASSERT(false, "Unknown ShaderDataType: {0}", (int)type);
	}

	static GLenum ShaderDataTypeToOpenGL(ShaderDataType type)
	{
		switch (type)
		{
			case BrickStacker::ShaderDataType::Float:	return GL_FLOAT;
			case BrickStacker::ShaderDataType::Vec2:	return GL_FLOAT;
			case BrickStacker::ShaderDataType::Vec3:	return GL_FLOAT;
			case BrickStacker::ShaderDataType::Vec4:	return GL_FLOAT;
			case BrickStacker::ShaderDataType::Int:		return GL_INT;
			case BrickStacker::ShaderDataType::IVec2:	return GL_INT;
			case BrickStacker::ShaderDataType::IVec3:	return GL_INT;
			case BrickStacker::ShaderDataType::IVec4:	return GL_INT;
			case BrickStacker::ShaderDataType::Uint:	return GL_UNSIGNED_INT;
			case BrickStacker::ShaderDataType::UVec2:	return GL_UNSIGNED_INT;
			case BrickStacker::ShaderDataType::UVec3:	return GL_UNSIGNED_INT;
			case BrickStacker::ShaderDataType::UVec4:	return GL_UNSIGNED_INT;
			case BrickStacker::ShaderDataType::Bool:	return GL_BOOL;
			case BrickStacker::ShaderDataType::BVec2:	return GL_BOOL;
			case BrickStacker::ShaderDataType::BVec3:	return GL_BOOL;
			case BrickStacker::ShaderDataType::BVec4:	return GL_BOOL;
			case BrickStacker::ShaderDataType::Double:	return GL_DOUBLE;
			case BrickStacker::ShaderDataType::DVec2:	return GL_DOUBLE;
			case BrickStacker::ShaderDataType::DVec3:	return GL_DOUBLE;
			case BrickStacker::ShaderDataType::DVec4:	return GL_DOUBLE;
			case BrickStacker::ShaderDataType::Mat2:	return GL_FLOAT;
			case BrickStacker::ShaderDataType::Mat3:	return GL_FLOAT;
			case BrickStacker::ShaderDataType::Mat4:	return GL_FLOAT;
		}

		BS_ASSERT(false, "Unknown ShaderDataType: {0}", (int)type);
	}

	struct BufferElement
	{
	public:
		std::string Name;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;
		bool Instanced;
		ShaderDataType Type;

		BufferElement(ShaderDataType type, const std::string& name, bool instanced = false, bool normalized = false)
			: Name{ name }, Type{ type }, Size{ ShaderDataTypeSize(type) }, Offset{ 0 }, Instanced{ instanced }, Normalized{ normalized }
		{}

		uint32_t GetComponentCount() const { return GetShaderDataTypeComponentCount(Type); };
		GLenum GetOpenGLType() const { return ShaderDataTypeToOpenGL(Type); };
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& Elements)
			: m_Elements{ Elements }
		{
			CalculateOffestsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; };
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; };

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); };
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); };
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); };
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); };
	private:
		void CalculateOffestsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
			}
			m_Stride = offset;
		}

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride{ 0 };
	};


	//-------------//
	//Vertex Buffer//
	//-------------//

	class VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create(const std::vector<float>& verticies, GLenum usage = GL_STATIC_DRAW) { return CreateRef<VertexBuffer>(verticies, usage); };
		VertexBuffer(const std::vector<float>& verticies, GLenum usage = GL_STATIC_DRAW);
		~VertexBuffer();;
		
		void UpdateBuffer(const std::vector<float>& verticies, GLenum usage = GL_STATIC_DRAW);
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; };
		const BufferLayout& GetLayout() const { return m_Layout; };

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	//------------//
	//Index Buffer//
	//------------//

	class IndexBuffer
	{
	public:
		static Ref<IndexBuffer> Create(const std::vector<uint32_t>& indicies) { return CreateRef<IndexBuffer>(indicies); };
		IndexBuffer(const std::vector<uint32_t>& indicies);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const;
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	//------------//
	//Vertex Array//
	//------------//

	enum class RenderingMode
	{
		Points = GL_POINTS,
		Lines = GL_LINES,
		Triangles = GL_TRIANGLES
	};

	class VertexArray
	{
	public:
		static Ref<VertexArray> Create() { return CreateRef<VertexArray>(); }
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

		RenderingMode renderingMode = RenderingMode::Triangles;
		float lineWidth = 1.0f;
	private:
		uint32_t m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}
