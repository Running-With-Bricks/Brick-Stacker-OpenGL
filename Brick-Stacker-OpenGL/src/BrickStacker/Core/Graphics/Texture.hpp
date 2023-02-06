#pragma once
#include "pch.hpp"

#include <glm/glm.hpp>

namespace BrickStacker
{
	class Texture
	{
	public:
		~Texture() {};

		void Bind();
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& filepath) { CreateRef<Texture2D>(filepath); };
		Texture2D(const std::string& filepath);

		const glm::vec2 GetSize() const { return m_Size; };

	private:
		uint32_t m_RendererID;
		std::string m_Filepath;
		glm::vec2 m_Size;
	};
}