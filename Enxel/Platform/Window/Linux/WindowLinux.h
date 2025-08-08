#pragma once

#include "../Window.h"
#include "SDL3/SDL.h"



class WindowLinux : public IWindow
{
public:
	WindowLinux(const WindowProperties& props);

	inline unsigned int GetWidth() const override { return m_Properties.width; }
	inline unsigned GetHeight() const override { return m_Properties.height; }
	inline SDL_Window* GetSDLWindow() override { return m_Window; }

	bool Tick();
	void Cleanup();

private:
	void Init(const WindowProperties& Properties);

	WindowProperties m_Properties;
	SDL_Window* m_Window;
};