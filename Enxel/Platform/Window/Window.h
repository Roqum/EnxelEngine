#pragma once
#include <SDL3/SDL_video.h>


struct WindowProperties
{

	const char* title;
	unsigned int width;
	unsigned int height;

	WindowProperties(const char* title = "Enxel Engine", 
		unsigned int width = 1000,
		unsigned int height = 800) 
		: title(title), width(width), height(height) 
	{
	}

};
class IWindow
{
public:
	virtual ~IWindow() {};

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned GetHeight() const = 0;

	virtual bool Tick() = 0;
	virtual void Cleanup() = 0;

	static IWindow* Create( const WindowProperties& props = WindowProperties());

	virtual SDL_Window* GetSDLWindow() = 0;

};