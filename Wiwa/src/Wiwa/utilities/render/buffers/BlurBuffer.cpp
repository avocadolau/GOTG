#include "wipch.h"
#include "BlurBuffer.h"

#include <glew.h>

namespace Wiwa {
	BlurBuffer::BlurBuffer()
	{
	}
	BlurBuffer::~BlurBuffer()
	{
		glDeleteFramebuffers(1, &m_FBO);
		glDeleteTextures(2, &m_ColorTexture);
	}
	void BlurBuffer::Init(int width, int height)
	{
		m_Init = true;

		m_Width = width;
		m_Height = height;
		glGenFramebuffers(1, &m_FBO);
		glGenTextures(2, &m_ColorTexture);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

			// also check if framebuffers are complete (no need for depth buffer)
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void BlurBuffer::Bind(bool clear)
	{
		glViewport(0, 0, m_Width, m_Height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		if (clear)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}
	void BlurBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void BlurBuffer::BindTexture()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
	}
	void BlurBuffer::Clear()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}