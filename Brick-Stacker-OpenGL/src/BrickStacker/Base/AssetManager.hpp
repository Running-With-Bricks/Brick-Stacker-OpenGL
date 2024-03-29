#pragma once
#include "BrickStacker/Core/Renderer/Renderer.hpp"

namespace BrickStacker
{
	class AssetManager
	{
	public:
		static AssetManager& Get()
		{
			static AssetManager AssetManager{};
			return AssetManager;
		}

		Ref<VertexArray> GetBrickModel() { return m_BrickModel; }
		Ref<VertexArray> GetBaseplateModel() { return m_BaseplateModel; }
		Ref<VertexArray> GetWireframeBrickModel() { return m_WireframeBrickModel; }
		Ref<VertexArray> GetWireframeBaseplateModel() { return m_WireframeBaseplateModel; }
		Ref<Shader> GetBrickShader() { return m_BrickShader; }
		Ref<Shader> GetBaseplateShader() { return m_BaseplateShader; }
	private:
		AssetManager();
		~AssetManager() = default;

		Ref<VertexArray> m_BrickModel;
		Ref<VertexArray> m_BaseplateModel;
		Ref<VertexArray> m_WireframeBrickModel;
		Ref<VertexArray> m_WireframeBaseplateModel;
		Ref<Shader> m_BrickShader;
		Ref<Shader> m_BaseplateShader;
	};
}

