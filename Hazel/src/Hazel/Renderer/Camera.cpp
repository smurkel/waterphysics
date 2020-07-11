#include "hzpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {



	Camera::Camera(float fov_angle, float aspectratio, float z_near, float z_far)
		: m_ProjectionMatrix(glm::perspective(fov_angle, aspectratio, z_near, z_far)), m_ViewMatrix(1.0f)
	{
		RecalculateViewMatrix();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateViewMatrix()
	{
		glm::vec3 XYZ = {
			cos(glm::radians(m_Position.y)) * sin(glm::radians(m_Position.z)) * m_Position.x,
			cos(glm::radians(m_Position.z)) * m_Position.x,
			sin(glm::radians(m_Position.y))* sin(glm::radians(m_Position.z))* m_Position.x,
		};

		m_ViewMatrix = glm::lookAt(XYZ + m_Focus, m_Focus, glm::vec3(0, 1, 0));
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}



}