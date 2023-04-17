#include <wipch.h>
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <Wiwa/core/Application.h>
#include <glew.h>
#include <gl/GL.h>

namespace Wiwa {
	const int IND_COUNT = 24;

	Camera::Camera()
	{
		// Initialize camera transform
		m_CameraPos = glm::vec3(0.0f, 0.0f, 1.5f);
		m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraRot = glm::vec3(0.0f, 0.0f, 0.0f);

		// Initialize camera distance planes
		m_NearPlaneDist = 0.1f;
		m_FarPlaneDist = 1000.0f;

		// Initialize camera view
		updateView();

		// Start camera as INVALID
		m_CameraType = CameraType::INVALID;
	}


	Camera::~Camera()
	{
		delete frameBuffer;
		delete shadowBuffer;
	}

	void Camera::updateView()
	{
		m_View = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
		UpdateFrustrum();
	}

	void Camera::setRotation(const glm::vec3 rot)
	{
		m_CameraRot = rot;

		glm::vec3 direction;
		direction.x = cos(glm::radians(rot.x)) * cos(glm::radians(rot.y));
		direction.y = sin(glm::radians(rot.y));
		direction.z = sin(glm::radians(rot.x)) * cos(glm::radians(rot.y));

		glm::vec3 front = glm::normalize(direction);
		setFront({ front.x, front.y, front.z });
	}

	void Camera::setPlanes(const float nearPlane, const float farPlane)
	{
		m_NearPlaneDist = nearPlane;
		m_FarPlaneDist = farPlane;
		if (m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, nearPlane, farPlane);
		UpdateFrustrum();
	}

	void Camera::setAspectRatio(const float aspectRatio)
	{
		m_AspectRatio = aspectRatio;

		if(m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, m_NearPlaneDist, m_FarPlaneDist);
		UpdateFrustrum();
	}

	void Camera::setResolution(const int width, const int height)
	{
		m_AspectRatio = width / (float)height;

		if (m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlaneDist, m_FarPlaneDist);
		else if(m_CameraType == CameraType::ORTHOGRAPHIC)
			m_Projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
		UpdateFrustrum();
	}

	void Camera::setFOV(const float fov)
	{
		m_FOV = fov;

		if (m_CameraType == CameraType::PERSPECTIVE)
			m_Projection = glm::perspective(glm::radians(fov), m_AspectRatio, m_NearPlaneDist, m_FarPlaneDist);
		UpdateFrustrum();
	}

	void Camera::setPosition(const glm::vec3 position)
	{
		m_CameraPos = position;

		updateView();
	}

	void Camera::setFront(const glm::vec3 front)
	{
		m_CameraFront = front;

		updateView();
	}

	void Camera::lookat(const glm::vec3 position)
	{
		m_CameraFront = position - m_CameraPos;

		m_View = glm::lookAt(m_CameraPos, position, m_CameraUp);
		UpdateFrustrum();
	}

	

	void Camera::SetPerspective(const float fov, const float aspectRatio, const float nearPlaneDistance, const float farPlaneDistance)
	{
		m_CameraType = CameraType::PERSPECTIVE;
		m_FOV = fov;
		m_Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, nearPlaneDistance, farPlaneDistance);

		m_AspectRatio = aspectRatio;
		m_NearPlaneDist = nearPlaneDistance;
		m_FarPlaneDist = farPlaneDistance;
		UpdateFrustrum();
		Size2i res = Application::Get().GetTargetResolution();

		//this buffer will have two color attatchements. 2 textures in a Fbo
		frameBuffer = new FrameBuffer();
		frameBuffer->Init(res.w, res.h, 2,true);

		shadowBuffer = new ShadowBuffer();
		shadowBuffer->Init();
	}

	void Camera::UpdateFrustrum()
	{
		frustrum = Math::Frustum(m_Projection * m_View);
	}

	void Camera::SetOrthographic(const int width, const int height, const float nearPlaneDistance, const float farPlaneDistance)
	{
		m_CameraType = CameraType::ORTHOGRAPHIC;

		m_Projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, nearPlaneDistance, farPlaneDistance);

		m_AspectRatio = width / (float)height;
		m_NearPlaneDist = nearPlaneDistance;
		m_FarPlaneDist = farPlaneDistance;

		frameBuffer = new FrameBuffer();
		frameBuffer->Init(width, height);
	}
	void Camera::DrawFrustrum()
	{
		//glLineWidth(5.0f);

		//// Generate frustum data
		//

		//glBindVertexArray(vao);

		//// Update frumstum data
		//glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_FrustumPoints), m_FrustumPoints);

		//// Draw frustum
		//glDrawElements(GL_LINES, (GLsizei)IND_COUNT, GL_UNSIGNED_INT, 0);

		//// Unbind buffers
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindVertexArray(0);

		//glLineWidth(1.0f);
	}
	void Camera::GetCornerPoints(glm::vec3* outPointArray)
	{
		Wiwa::Size2i& res = Wiwa::Application::Get().GetTargetResolution();
		float verticalFOV = 2 * glm::atan(glm::tan(glm::radians(m_FOV) / 2) * ((float)res.h / (float)res.w));
		float tanhfov = glm::tan(glm::radians(m_FOV) * 0.5f);
		float tanvfov = glm::tan(verticalFOV * 0.5f);
		float frontPlaneHalfWidth = tanhfov * m_NearPlaneDist;
		float frontPlaneHalfHeight = tanvfov * m_NearPlaneDist;
		float farPlaneHalfWidth = tanhfov * m_FarPlaneDist;
		float farPlaneHalfHeight = tanvfov * m_FarPlaneDist;

		glm::vec3 right = glm::cross(m_CameraFront, m_CameraUp);

		glm::vec3 nearCenter = m_CameraPos + m_CameraFront * m_NearPlaneDist;
		glm::vec3 nearHalfWidth = frontPlaneHalfWidth * right;
		glm::vec3 nearHalfHeight = frontPlaneHalfHeight * m_CameraUp;
		outPointArray[0] = nearCenter - nearHalfWidth - nearHalfHeight;
		outPointArray[1] = nearCenter + nearHalfWidth - nearHalfHeight;
		outPointArray[2] = nearCenter - nearHalfWidth + nearHalfHeight;
		outPointArray[3] = nearCenter + nearHalfWidth + nearHalfHeight;

		glm::vec3 farCenter = m_CameraPos + m_CameraFront * m_FarPlaneDist;
		glm::vec3 farHalfWidth = farPlaneHalfWidth * right;
		glm::vec3 farHalfHeight = farPlaneHalfHeight * m_CameraUp;
		outPointArray[4] = farCenter - farHalfWidth - farHalfHeight;
		outPointArray[5] = farCenter + farHalfWidth - farHalfHeight;
		outPointArray[6] = farCenter - farHalfWidth + farHalfHeight;
		outPointArray[7] = farCenter + farHalfWidth + farHalfHeight;
	}
}