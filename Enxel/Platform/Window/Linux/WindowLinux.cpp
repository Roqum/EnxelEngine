#include "WindowLinux.h"

IWindow* IWindow::Create(const WindowProperties& props)
{
	return new WindowLinux(props);
}

WindowLinux::WindowLinux(const WindowProperties& props)
{
	Init(props);
}

bool WindowLinux::Tick()
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

void WindowLinux::Cleanup()
{
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void WindowLinux::Init(const WindowProperties& props)
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
