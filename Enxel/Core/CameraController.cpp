#include "CameraController.h"



namespace Enxel
{
	CameraController::CameraController(Camera* camera) : m_Camera(camera)
	{

	}

	void CameraController::OnUpdate(float deltaTime)
	{
		float speed = 0.2f;

		// Movement
		if (Input::IsKeyDown(KeyCode::W))
		{
			m_Camera->MoveForward(speed * deltaTime);
		}
		else if (Input::IsKeyDown(KeyCode::S))
		{
			m_Camera->MoveForward(-speed * deltaTime);
		}
		if (Input::IsKeyDown(KeyCode::A))
		{
			m_Camera->MoveSide(-speed * deltaTime);
		}
		else if (Input::IsKeyDown(KeyCode::D))
		{
			m_Camera->MoveSide(speed * deltaTime);
		}
		if (Input::IsKeyDown(KeyCode::Q))
		{
			m_Camera->MoveUp(-speed * deltaTime);
		}
		else if (Input::IsKeyDown(KeyCode::E))
		{
			m_Camera->MoveUp(speed * deltaTime);
		}
	}


}
