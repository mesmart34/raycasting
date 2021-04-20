#include "Window.h"

Window::Window(const WindowProps& props) : m_props(props)
{
	auto windowFlags = (Uint32)(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	m_window = SDL_CreateWindow("Wolf3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, windowFlags);
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
