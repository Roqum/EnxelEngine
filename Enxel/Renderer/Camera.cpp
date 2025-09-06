#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Enxel
{
	Camera::Camera() :
		m_Projection(glm::perspective(m_FOV,  m_AspectRatio, m_NearClip, m_FarClip))
	{
		m_ForwardDirection = glm::vec3(0, 0, -1);
		m_Position = glm::vec3(0, 0, 6);
		RecalculateView();
	}

	void Camera::OnUpdate(float deltaTime)
	{

	}

	void Camera::MoveForward(float speed)
	{
		constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
		glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

		m_Position += m_ForwardDirection * speed;
		RecalculateView();
	}

	void Camera::MoveUp(float speed)
	{
		constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
		glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

		m_Position += upDirection * speed;
		RecalculateView();
	}

	void Camera::MoveSide(float speed)
	{
		constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
		glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

		m_Position += rightDirection * speed;
		RecalculateView();
	}


	void Camera::RecalculateView()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0, 1, 0));
	}
}
