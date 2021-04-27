#pragma once
#include "SDL\SDL.h"
#include "SDL\SDL_events.h"
#include <string>
#include "Base.h"

struct WindowProps
{
	WindowProps(const int width = 800, const int height = 600, const std::string& title = "Raycaster") : Width(width), Height(height), Title(title) {}
	int Width;
	int Height;
	std::string Title;
};


class Window
{
public:
	Window(const WindowProps& props = WindowProps());
	~Window();

	SDL_Window* GetSDLPtr();
	static int SDLCALL Resize(void* data, SDL_Event* ev);

	int GetWidth() const;
	int GetHeight() const;

private:
	SDL_Window* m_window;
	WindowProps m_props;
};

