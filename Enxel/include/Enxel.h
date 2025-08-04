#pragma once

#include <memory>
#include "Renderer/IRenderer.h"

void* operator new (size_t size)
{
	return malloc(size);
}

void operator delete (void* mem)
{
	free(mem);
}


#ifdef ENXEL_BUILD
#define ENXEL_API __declspec(dllexport)
#else
#define ENXEL_API __declspec(dllimport)
#endif

class ENXEL_API Enxel
{
public:
	
	void StartEngine();

protected:
	IRenderer* renderer;
};