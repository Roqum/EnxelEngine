#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() :
	m_Projection(glm::perspective(m_FOV,  m_AspectRatio, m_NearClip, m_FarClip)),
	m_View(glm::lookAt(glm::vec3(60.0f, 60.0f, 60.0f), glm::vec3(16.0f, 16.0f, 16.0f), glm::vec3(0.0f, 1.0f, 0.0f))) 
{

}
