#include <wipch.h>
#include "FrameBuffer.h"

#include <glew.h>
#include <Wiwa/utilities/Log.h>

namespace Wiwa {
	FrameBuffer::FrameBuffer()
	{
		m_Init = false;
	}

	FrameBuffer::~FrameBuffer()
	{
		if (m_Init) {
			//glDeleteTextures(1, &m_ColorBufferTexture);
			glDeleteRenderbuffers(1, &m_RBO);
			//glDeleteFramebuffers(1, &m_FBOs);

			for (int i = 0; i < m_FBOs.size(); i++)
			{
				glDeleteFramebuffers(1, &m_FBOs[i]);
			}
			for (int i = 0; i < m_ColorBuffers.size(); i++)
			{
				glDeleteFramebuffers(1, &m_ColorBuffers[i]);
			}
		}
	}

	void FrameBuffer::Init(int width, int height, int n_Textures, bool depth)
	{
		m_Init = true;		

		m_Width = width;
		m_Height = height;
		// FRAMEBUFFER
		m_FBOs.resize(1);
		glGenFramebuffers(1, &m_FBOs[0]);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[0]);

		m_ColorBuffers.resize(n_Textures);
		//add 2 color buffer, one for normal rendering and the other for bloom
		glGenTextures(m_ColorBuffers.size(), &m_ColorBuffers[0]);
		for (unsigned int i = 0; i < m_ColorBuffers.size(); i++)
		{
	
			glBindTexture(GL_TEXTURE_2D, m_ColorBuffers[i]);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			// attach texture to framebuffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorBuffers[i], 0);
		}

		if (depth) {
			// Render buffer object for depth
			glGenRenderbuffers(1, &m_RBO);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
		}

		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		m_Atachtments.push_back(GL_COLOR_ATTACHMENT0);
		m_Atachtments.push_back(GL_COLOR_ATTACHMENT1);
		glDrawBuffers(m_Atachtments.size(),&m_Atachtments[0]);

		// Check framebuffer status
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			WI_CORE_ASSERT_MSG("Framebuffer not completed")
		}
		else
			WI_CORE_INFO("Framebuffer completed");
	

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FrameBuffer::InitBlur(int width, int height)
	{
		m_Init = true;

		m_Width = width;
		m_Height = height;

		m_FBOs.resize(2);
		m_ColorBuffers.resize(2);
		glGenFramebuffers(2, &m_FBOs[0]);
		glGenTextures(2, &m_ColorBuffers[0]);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[i]);
			glBindTexture(GL_TEXTURE_2D, m_ColorBuffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffers[i], 0);

			// also check if framebuffers are complete (no need for depth buffer)
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;

		
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FrameBuffer::Init(int width, int height, bool depth)
	{
		m_Init = true;		

		m_Width = width;
		m_Height = height;
		m_FBOs.resize(1);
		// FRAMEBUFFER
		glGenFramebuffers(1, &m_FBOs[0]);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[0]);

		m_ColorBuffers.resize(1);
		// Color texture
		glGenTextures(1, &m_ColorBuffers[0]);
		glBindTexture(GL_TEXTURE_2D, m_ColorBuffers[0]);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffers[0], 0);
		
		if (depth) {
			// Render buffer object for depth
			glGenRenderbuffers(1, &m_RBO);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
		}

		// Check framebuffer status
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			WI_CORE_ASSERT_MSG("Framebuffer not completed")
		}
		else
			WI_CORE_INFO("Framebuffer completed");
	

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Bind(bool clear)
	{	
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[0]);
		//glEnable(GL_FRAMEBUFFER_SRGB);
		if (clear) {
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}


	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_FRAMEBUFFER_SRGB);
	}

	void FrameBuffer::Clear()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[0]);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	inline std::vector<uint32_t> FrameBuffer::getColorBuffers()
	{
		return m_ColorBuffers;
	}
}