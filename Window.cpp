#include "Window.h"

Window::Window(const WindowProps& props) : m_props(props)
{
	auto windowFlags = (Uint32)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
	m_window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, props.Width, props.Height, windowFlags);
}

Window::~Window()
{
	SDL_DestroyWindow(m_window);
}

SDL_Window* Window::GetSDLPtr() const
{
	return m_window;
}

int Window::GetWidth() const
{
    return m_props.Width;
}

int Window::GetHeight() const
{
    return m_props.Height;
}
