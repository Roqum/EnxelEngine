#pragma once

#include "../Window.h"
#include "SDL3/SDL.h"



class WindowWin : public IWindow
{
public:
	WindowWin(const WindowProperties& props);

	inline unsigned int GetWidth() const override { return Properties.Width; };
	inline unsigned GetHeight() const override { return Properties.Height; };
	SDL_Window* GetSDLWindow() override;

	bool Tick();
	void Cleanup();

private:
	void Init(const WindowProperties& Properties);

	WindowProperties Properties;
	SDL_Window* window;
};