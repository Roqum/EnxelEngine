#pragma once

#include "Input.h"
#include "../Renderer/Camera.h"


namespace Enxel
{
	class CameraController
	{
	public:
		CameraController() = default;
		CameraController(Camera* m_Camera);
		virtual ~CameraController() = default;
		void OnUpdate(float deltaTime);

	private:
		Camera* m_Camera;
	};

}