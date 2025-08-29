#pragma once

#include "glm/glm.hpp"


class Camera
{
public:
	Camera();
	virtual ~Camera() = default;

	const glm::mat4& GetProjection() const { return m_Projection; }
	const glm::mat4& GetView() const { return m_View; }

private:

	void UpdateProjection();
	void UpdateView();

	float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

	glm::mat4 m_Projection;
	glm::mat4 m_View;
	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };



};