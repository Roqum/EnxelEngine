#pragma once

#include <memory>
#include "Renderer/RendererAPI.h"
#include "Renderer/Buffer.h"
#include "Core/Camera.h"
#include "Platform/Window/Window.h"


/*
void* operator new (size_t size)
{
	return malloc(size);
}

void operator delete (void* mem)
{
	free(mem);
}
*/
namespace Enxel
{
	class Enxel
	{
	public:
		Enxel();
		~Enxel();
		static Enxel& Get();

	
		void StartEngine();
		void Update(float deltaTime);

		inline IWindow& GetWindow() const { return *m_Window; }


	protected:
		std::unique_ptr<IWindow> m_Window;
		std::unique_ptr<IRenderer> m_Renderer;
		Camera m_Camera;

		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

	private:
	

	};
}