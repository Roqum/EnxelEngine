#pragma once


struct WindowProperties
{

	const char* Title;
	unsigned int Width;
	unsigned int Height;

	WindowProperties(const char* title = "Enxel Engine", 
		unsigned int width = 800,
		unsigned int height = 600) 
		: Title(title), Width(width), Height(height) 
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