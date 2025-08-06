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
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void WindowWin::Init(const WindowProperties& props)
{
	m_Properties.title = props.title;
	m_Properties.width = props.width;
	m_Properties.height = props.height;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	}

	m_Window = SDL_CreateWindow(
		m_Properties.title,
		m_Properties.width, m_Properties.height,
		SDL_WINDOW_VULKAN                 
	);

	if (!m_Window) {
		SDL_Quit();
	}
	bool running = true;
	SDL_Event event;

}
