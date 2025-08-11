#pragma once

#include <memory>
#include "Renderer/RendererAPI.h"
#include "Renderer/Buffer.h"
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
	
		void StartEngine();

	protected:
		std::unique_ptr<IWindow> m_Window;
		std::unique_ptr<IRenderer> m_Renderer;

		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

	};
}