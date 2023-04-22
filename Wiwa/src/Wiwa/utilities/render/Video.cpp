#include <wipch.h>
#include "Video.h"

#include <glew.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>
namespace Wiwa {
	Video::Video() : m_TextureId(-1), m_IsPlaying(true)
	{

	}

	Video::~Video()
	{
		if (m_TextureId != uint32_t(-1)) {
			glDeleteTextures(1, &m_TextureId);
		}
	}

	bool Video::Init(std::string videoPath, byte* data)
	{
		m_Cap = cv::VideoCapture(videoPath);
		if (!m_Cap.isOpened())
		{

			WI_CORE_ERROR("Video file not loaded!");
			return false;

		}
		m_Cap >> m_Frame;
		int videoWidth = m_Frame.cols;
		int videoHeight = m_Frame.rows;
		m_VideoImage = cvMat2TexInput(m_Frame);
		glGenTextures(1, &m_TextureId);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			videoWidth,
			videoHeight,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			m_VideoImage);

		m_ImageSize.w = videoWidth;
		m_ImageSize.h = videoHeight;

		m_FrameTime = int(1000 / GetFrameRate()); 
		m_LastTime = glfwGetTime(); 

		return true;
	}

	void Video::Play()
	{
		m_IsPlaying = true;
	}

	void Video::Stop()
	{
		m_IsPlaying = false;
	}

	void Video::Replay()
	{
		m_Cap.set(cv::CAP_PROP_POS_FRAMES, 0);
	}

	bool Video::Update()
	{
		if (!m_IsPlaying)
			return false;
		m_CurrentTime = glfwGetTime(); 
		m_ElapsedTime = (m_CurrentTime - m_LastTime) * 1000; 
		WI_CORE_INFO(m_ElapsedTime);
		if (m_ElapsedTime >= m_FrameTime) {

			m_Cap >> m_Frame;

			if (m_Frame.empty())
			{
				return false;
			}
			m_VideoImage = cvMat2TexInput(m_Frame);

			if (m_VideoImage)
			{
				glDeleteTextures(1, &m_TextureId);

				glGenTextures(1, &m_TextureId);
				glBindTexture(GL_TEXTURE_2D, m_TextureId);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_ImageSize.w, m_ImageSize.h, 0, GL_RGB, GL_UNSIGNED_BYTE, m_VideoImage);

			}

			m_LastTime = m_CurrentTime;
		}
		
		return true;
	}

	unsigned char* Video::cvMat2TexInput(cv::Mat& img)
	{
		cvtColor(img, img, cv::COLOR_BGR2RGB);
		cv::rotate(img, img, cv::ROTATE_90_CLOCKWISE);
		transpose(img, img);
		flip(img, img, 0);
		return img.data;
	}
}