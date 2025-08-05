#pragma once

#include <memory>
#include "Renderer/RendererAPI.h"
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

class Enxel
{
public:
	
	void StartEngine();

protected:
	IRenderer* renderer;
};