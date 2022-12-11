#pragma once

#include "glad/glad.h"

#include <stdint.h>

namespace BrickStacker
{
	enum class FramebufferFormat
	{

	};

	struct FramebufferSpecifications
	{
		uint32_t Width, Height = 1024;

	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecifications& specs);
		~Framebuffer();

		void Bind();
		void Unbind();
	private:
		uint32_t m_RendererID;
	};
}
