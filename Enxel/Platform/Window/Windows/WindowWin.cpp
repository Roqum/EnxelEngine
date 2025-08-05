#include "WindowWin.h"

IWindow* IWindow::Create(const WindowProperties& props)
{
	return new WindowWin(props);
}

WindowWin::WindowWin(const WindowProperties& props)
{
	Init(props);
}

bool WindowWin::Tick()
{
	bool running = true;
	SDL_Event event;

	SDL_PollEvent(&event);
	if (event.type == SDL_EVENT_QUIT) {
		// TODO: Trigger close window event
		running = false;
	}
	return running;
}

void WindowWin::Cleanup()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void WindowWin::Init(const WindowProperties& props)
{
	Properties.Title = props.Title;
	Properties.Width = props.Width;
	Properties.Height = props.Height;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	}

	SDL_Window* window = SDL_CreateWindow(
		Properties.Title.c_str(),
		Properties.Width, Properties.Height,                         
		SDL_WINDOW_VULKAN                 
	);

	if (!window) {
		SDL_Quit();
	}
	bool running = true;
	SDL_Event event;


	
}
