#include <wipch.h>

#include "Text.h"

#include "../vendor/gli/gli/gli.hpp"
#include <glew.h>

namespace Wiwa {
	Text::Text() : m_TextureId(-1)
	{

	}

	Text::~Text()
	{
		if (m_TextureId != uint32_t(-1)) {
			glDeleteTextures(1, &m_TextureId);
		}
	}

	bool Text::Init(int width, int height, byte* data)
	{
		int w = width, h = height;


		glGenTextures(1, &m_TextureId);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_LUMINANCE,
			width,
			height,
			0,
			GL_LUMINANCE,
			GL_UNSIGNED_BYTE,
			data);

	


		m_ImageSize.x = w;
		m_ImageSize.y = h;


		return true;
	}
}