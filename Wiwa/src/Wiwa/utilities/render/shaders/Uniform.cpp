#include "wipch.h"
#include "Uniform.h"
#include <Wiwa/utilities/time/Time.h>
#include <glew.h>

#include <glm/glm.hpp>

namespace Wiwa {
	//Sampler2D x : OpenGL id y: ResourceId to retrieve the path when saving
	void Uniform::sendToShader(uint16_t shaderProgram, int& textureId)
	{
		if (m_UniformID == NULL)
			m_UniformID = glGetUniformLocation(shaderProgram, name.c_str());

		switch (m_Type)
		{
		case Wiwa::UniformType::Bool:
			glUniform1i(m_UniformID, *static_cast<bool*>(m_Data));
			break;
		case Wiwa::UniformType::Int:
			glUniform1i(m_UniformID, *static_cast<int*>(m_Data));
			break;
		case Wiwa::UniformType::Uint:
			glUniform1ui(m_UniformID, *static_cast<unsigned int*>(m_Data));
			break;
		case Wiwa::UniformType::Float:
			glUniform1f(m_UniformID, *static_cast<float*>(m_Data));
			break;
		case Wiwa::UniformType::fVec2:
			glUniform2fv(m_UniformID, 1, static_cast<float*>(m_Data));
			break;
		case Wiwa::UniformType::fVec3:
			glUniform3fv(m_UniformID, 1, static_cast<float*>(m_Data));
			break;
		case Wiwa::UniformType::fVec4:
			glUniform4fv(m_UniformID, 1, static_cast<float*>(m_Data));
			break;
		case Wiwa::UniformType::Mat2:
			glUniformMatrix2fv(m_UniformID, 1, GL_FALSE, static_cast<float*>(m_Data));
			break;
		case Wiwa::UniformType::Mat3:
			glUniformMatrix3fv(m_UniformID, 1, GL_FALSE, static_cast<float*>(m_Data));
			break;
		case Wiwa::UniformType::Mat4:
			glUniformMatrix4fv(m_UniformID, 1, GL_FALSE, static_cast<float*>(m_Data));
			break;
		case Wiwa::UniformType::Time:
		{
			glUniform1f(m_UniformID, Wiwa::Time::GetRealTimeSinceStartup());
			setData(Wiwa::Time::GetRealTimeSinceStartup(), m_Type);
		}break;
		case Wiwa::UniformType::Sampler2D:
		{
			glActiveTexture(textureId);
			glBindTexture(GL_TEXTURE_2D, ((SamplerData*)m_Data)->tex_id);
			glUniform1i(m_UniformID, textureId - GL_TEXTURE0);
			textureId++;

		}break;
		default:
			break;
		}
	}

	void Uniform::setEmptyData()
	{
		switch (m_Type)
		{
		case Wiwa::UniformType::Bool:
			setData(false, m_Type);
			break;
		case Wiwa::UniformType::Int:
			setData(0, m_Type);
			break;
		case Wiwa::UniformType::Uint:
			setData(0, m_Type);
			break;
		case Wiwa::UniformType::Float:
			setData(1.0f, m_Type);
			break;
		case Wiwa::UniformType::fVec2:
			setData(glm::vec2(), m_Type);
			break;
		case Wiwa::UniformType::fVec3:
			setData(glm::vec3(0.0f), m_Type);
			break;
		case Wiwa::UniformType::fVec4:
			setData(glm::vec4(1.0f), m_Type);
			break;
		case Wiwa::UniformType::Mat2:
			setData(glm::mat2(), m_Type);
			break;
		case Wiwa::UniformType::Mat3:
			setData(glm::mat3(), m_Type);
			break;
		case Wiwa::UniformType::Mat4:
			setData(glm::mat4(), m_Type);
			break;
		case Wiwa::UniformType::Time:
			setData(0.0f, m_Type);
			break;
		case Wiwa::UniformType::Sampler2D:
			setData(SamplerData{}, m_Type);
			break;
		default:
			break;
		}
	}

}