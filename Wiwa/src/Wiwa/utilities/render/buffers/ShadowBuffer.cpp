#include <wipch.h>
#include "ShadowBuffer.h"

#include <glew.h>

Wiwa::ShadowBuffer::ShadowBuffer() 
	: m_Init(false)
{
}

Wiwa::ShadowBuffer::~ShadowBuffer()
{
	if (m_Init) {
		glDeleteTextures(1, &m_ShadowMap);
		glDeleteFramebuffers(1, &m_DBO);
		glDeleteTextures(1, &m_ShadowMap);
	}
}

void Wiwa::ShadowBuffer::Init(uint32_t width, uint32_t height)
{
	m_Init = true;

	m_ShadowHeight = height;
	m_ShadowWidth = width;

	glGenBuffers(1, &m_DBO);

	glGenTextures(1, &m_ShadowMap);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_ShadowHeight, m_ShadowWidth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_DBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMap, 0);

	glNamedFramebufferDrawBuffer(m_DBO, GL_NONE);
	glNamedFramebufferReadBuffer(m_DBO, GL_NONE);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		WI_CORE_ASSERT_MSG("Shadow buffer not completed")
	}
	else
		WI_CORE_INFO("Shadow buffer completed");

	
}

void Wiwa::ShadowBuffer::Bind(bool clear)
{
	glViewport(0, 0, m_ShadowWidth, m_ShadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_DBO);
	if (clear)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}

void Wiwa::ShadowBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Wiwa::ShadowBuffer::BindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
}

void Wiwa::ShadowBuffer::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_DBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
