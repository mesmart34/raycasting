#include "Window.h"

Window::Window(const WindowProps& props) : m_props(props)
{
	auto windowFlags = (Uint32)(SDL_WINDOW_RESIZABLE);
	m_window = SDL_CreateWindow("Ray casting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, props.Width, props.Height, windowFlags);
	SDL_AddEventWatch(Window::Resize, this);
}

Window::~Window()
{
	SDL_DestroyWindow(m_window);
}

SDL_Window* Window::GetSDLPtr()
{
	return m_window;
}

int SDLCALL Window::Resize(void* data, SDL_Event* ev)
{
	if(ev->window.event == SDL_WINDOWEVENT_RESIZED)
	if (auto window = static_cast<Window*>(data))
	{
		window->m_props.Width = ev->window.data1;
		window->m_props.Height = ev->window.data2;
		return 0;
	}
	return -1;
}

int Window::GetWidth() const
{
    return m_props.Width;
}

int Window::GetHeight() const
{
    return m_props.Height;
}
