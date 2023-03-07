#pragma once
#include "pch.hpp"

#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <BrickStacker/Core/Graphics/Texture.hpp>

namespace BrickStacker
{
	class Shader
	{
	public:
		static Ref<Shader> Create(const std::string& vert, const std::string& frag) { return CreateRef<Shader>(vert, frag); };
		Shader(const std::string& vert, const std::string& frag);
		~Shader();

		void Bind() const;
		void Unbind() const;

		uint32_t GetRendererID() { return m_RendererID; };

		void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const;
		void SetUniformTexture(const std::string& name, uint32_t slot = 0) const;
	private:
		uint32_t m_RendererID;
	};
}
