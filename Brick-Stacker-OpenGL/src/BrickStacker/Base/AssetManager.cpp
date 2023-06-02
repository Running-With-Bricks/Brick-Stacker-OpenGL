#include "pch.hpp"
#include "AssetManager.hpp"

namespace BrickStacker
{
	AssetManager::AssetManager()
	{
		{
			m_BrickModel = VertexArray::Create();

			//Cube Verts
			std::vector<float> CubeVerticies =
			{
				//Top
				-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1,
				 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1,
				 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1,
				-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1,
				//Bottom
				-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 2,
				 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 2,
				 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 2,
				-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 2,
				//Left
				-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0,
				-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0,
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0,
				-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0,
				//Right
				 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0,
				 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0,
				 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0,
				 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0,
				//Front
				 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0,
				-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0,
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0,
				 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0,
				//Back
				 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0,
				-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0,
				-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0,
				 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0,
			};

			//Cube Indicies
			std::vector<uint32_t> CubeIndicies =
			{
				0, 2, 1,
				2, 0, 3,

				5, 6, 4,
				7, 4, 6,

				8, 10, 9,
				10, 8, 11,

				13, 14, 12,
				15, 12, 14,

				17, 18, 16,
				19, 16, 18,

				20, 22, 21,
				22, 20, 23,
			};

			//Specifying Vertex properties
			//Vec3 of floats for the position and Vec4 of floats for Color
			BufferLayout CubeLayout =
			{
				{ ShaderDataType::Vec3, "Position" },
				{ ShaderDataType::Vec2, "TexCoord" },
				{ ShaderDataType::Float, "TexIndex" },
			};

			BufferLayout InstancedLayout =
			{
				{ ShaderDataType::Vec4, "Color", true },
				{ ShaderDataType::Vec2, "TilingFactor", true},
				{ ShaderDataType::Vec3, "Transform1", true },
				{ ShaderDataType::Vec3, "Transform2", true },
				{ ShaderDataType::Vec3, "Transform3", true },
				{ ShaderDataType::Vec3, "Transform4", true },
			};

			//Creating objects which will hold cube data

			Ref<VertexBuffer> cubeVertexBuffer = VertexBuffer::Create(CubeVerticies);
			Ref<VertexBuffer> cubeInstancedVertexBuffer = VertexBuffer::Create({}, GL_STREAM_DRAW);
			Ref<IndexBuffer> cubeIndexBuffer = IndexBuffer::Create(CubeIndicies);

			//Setting the layout
			cubeVertexBuffer->SetLayout(CubeLayout);
			cubeInstancedVertexBuffer->SetLayout(InstancedLayout);

			//Binding VertexBuffer and IndexBuffer to VertexArray
			m_BrickModel->AddVertexBuffer(cubeVertexBuffer);
			m_BrickModel->AddVertexBuffer(cubeInstancedVertexBuffer);
			m_BrickModel->SetIndexBuffer(cubeIndexBuffer);
		}

		{
			m_BaseplateModel = VertexArray::Create();

			//Cube Verts
			std::vector<float> CubeVerticies =
			{
				//Top
				-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1,
				 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1,
				 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1,
				-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1,
			};

			//Cube Indicies
			std::vector<uint32_t> CubeIndicies =
			{
				0, 2, 1,
				2, 0, 3,
			};

			//Specifying Vertex properties
			//Vec3 of floats for the position and Vec4 of floats for Color
			BufferLayout CubeLayout =
			{
				{ ShaderDataType::Vec3, "Position" },
				{ ShaderDataType::Vec2, "TexCoord" },
				{ ShaderDataType::Float, "TexIndex" },
			};

			BufferLayout InstancedLayout =
			{
				{ ShaderDataType::Vec4, "Color", true },
				{ ShaderDataType::Float, "Scale", true}
			};

			//Creating objects which will hold cube data

			Ref<VertexBuffer> cubeVertexBuffer = VertexBuffer::Create(CubeVerticies);
			Ref<VertexBuffer> cubeInstancedVertexBuffer = VertexBuffer::Create({}, GL_STREAM_DRAW);
			Ref<IndexBuffer> cubeIndexBuffer = IndexBuffer::Create(CubeIndicies);

			//Setting the layout
			cubeVertexBuffer->SetLayout(CubeLayout);
			cubeInstancedVertexBuffer->SetLayout(InstancedLayout);

			//Binding VertexBuffer and IndexBuffer to VertexArray
			m_BaseplateModel->AddVertexBuffer(cubeVertexBuffer);
			m_BaseplateModel->AddVertexBuffer(cubeInstancedVertexBuffer);
			m_BaseplateModel->SetIndexBuffer(cubeIndexBuffer);
		}

		{
			m_BrickShader = Shader::Create("shaders/brick.vert", "shaders/brick.frag");
			
			m_BrickShader->Bind();
			m_BrickShader->SetUniformTexture("TopTexture", 0);
			m_BrickShader->SetUniformTexture("BottomTexture", 1);
			m_BrickShader->Unbind();
		}


		{
			m_BaseplateShader = Shader::Create("shaders/baseplate.vert", "shaders/baseplate.frag");
			
			m_BaseplateShader->Bind();
			m_BaseplateShader->SetUniformTexture("TopTexture", 0);
			m_BaseplateShader->Unbind();
		}
	}
}