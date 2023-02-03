#pragma once
#include "pch.hpp"

#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace BrickStacker
{
	class Shader
	{
	public:
		static Ref<Shader> Create(const std::string& vertSrc, const std::string& fragSrc) { return CreateRef<Shader>(vertSrc, fragSrc); };
		Shader(const std::string& vertSrc, const std::string& fragSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const;
	private:
		uint32_t m_RendererID;
	};
}
