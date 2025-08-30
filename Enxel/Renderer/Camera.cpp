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
		bool moved = false;

		constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
		glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);
		float speed = 1.0f;

		// Movement
		if (Input::IsKeyDown(KeyCode::W))
		{
			m_Position += m_ForwardDirection * speed * deltaTime;
			moved = true;
		}
		else if (Input::IsKeyDown(KeyCode::S))
		{
			m_Position -= m_ForwardDirection * speed * deltaTime;		
			moved = true;
		}
		if (Input::IsKeyDown(KeyCode::A))
		{
			m_Position -= rightDirection * speed * deltaTime;
			moved = true;
		}
		else if (Input::IsKeyDown(KeyCode::D))
		{
			m_Position += rightDirection * speed * deltaTime;
			moved = true;
		}
		if (Input::IsKeyDown(KeyCode::Q))
		{
			m_Position -= upDirection * speed * deltaTime;
			moved = true;
		}
		else if (Input::IsKeyDown(KeyCode::E))
		{
			m_Position += upDirection * speed * deltaTime;
			moved = true;
		}

		
		if (moved)
		{
			RecalculateView();
		}
	}

	void Camera::RecalculateView()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0, 1, 0));
	}
}
