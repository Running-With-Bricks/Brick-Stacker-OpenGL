#include "FrameBuffer.hpp"

namespace BrickStacker
{
	Framebuffer::Framebuffer(const FramebufferSpecifications& specs)
	{
		glGenFramebuffers(1, &m_RendererID);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}
}
