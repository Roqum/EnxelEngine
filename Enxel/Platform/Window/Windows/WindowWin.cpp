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
	return false;
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
