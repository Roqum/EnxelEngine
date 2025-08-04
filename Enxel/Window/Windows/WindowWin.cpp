#include "WindowWin.h"

IWindow* IWindow::Create(const WindowProperties& props)
{
	return new WindowWin(props);
}

WindowWin::WindowWin(const WindowProperties& props)
{
	Init(props);
}

void WindowWin::Init(const WindowProperties& props)
{
	Properties.Title = props.Title;
	Properties.Width = props.Width;
	Properties.Height = props.Height;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	}

	SDL_Window* window = SDL_CreateWindow(
		"SDL Window",                     // title
		Properties.Width, Properties.Height,                         // width, height
		SDL_WINDOW_OPENGL                 // flags (optional: SDL_WINDOW_RESIZABLE, etc.)
	);

	if (!window) {
		SDL_Quit();
	}
	bool running = true;
	SDL_Event event;

	while (running) 
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_EVENT_QUIT) {
			running = false;
		}

		// Clear screen with black
		SDL_Delay(16); // ~60 FPS
	}

	// Cleanup
	SDL_DestroyWindow(window);
	SDL_Quit();
}
