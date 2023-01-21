#pragma once

#include "BrickStacker/Core/Core.hpp"

#include "glad/glad.h"

#include <stdint.h>

namespace BrickStacker
{
	enum class FramebufferFormat
	{

	};

	struct FramebufferSpecifications
	{
		uint32_t Width = 1024;
		uint32_t Height = 1024;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(const FramebufferSpecifications& specs) { return CreateRef<Framebuffer>(specs); };
		Framebuffer(const FramebufferSpecifications& specs);
		~Framebuffer();

		void Resize(uint32_t Width, uint32_t Height);
		void Invalidate();
		uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; };

		FramebufferSpecifications& GetSpecifications() { return m_Specification; };
		const FramebufferSpecifications& GetSpecifications() const { return m_Specification; };

		void Bind();
		void Unbind();
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		FramebufferSpecifications m_Specification;
	};
}
