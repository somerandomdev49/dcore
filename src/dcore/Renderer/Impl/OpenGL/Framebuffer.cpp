#include <dcore/Renderer/Impl/OpenGL/Framebuffer.hpp>
#include <dcore/Core/Log.hpp>

namespace dcore::graphics::impl::opengl
{
	void Framebuffer::Create(glm::vec2 size)
	{
		Size_ = size;
		glGenFramebuffers(1, &FBO_);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_);

		glGenTextures(1, &Texture_);
		glBindTexture(GL_TEXTURE_2D, Texture_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Size_.x, Size_.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture_, 0);

		glGenRenderbuffers(1, &RBO_);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Size_.x, Size_.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			DCORE_LOG_ERROR << "Framebuffer is not complete!";

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void Framebuffer::ReSize(glm::vec2 newSize)
	{
		Size_ = newSize;
		glBindTexture(GL_TEXTURE_2D, Texture_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Size_.x, Size_.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture_, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, RBO_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Size_.x, Size_.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_);
	}

	void Framebuffer::Delete()
	{
		glDeleteFramebuffers(1, &FBO_);
		glDeleteTextures(1, &Texture_);
		glDeleteRenderbuffers(1, &RBO_);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
	}

	void Framebuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
