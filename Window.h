#pragma once
#include "SDL\SDL.h"
#include "SDL\SDL_events.h"
#include <string>
#include "Base.h"
#include <iostream>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include <functional>

struct WindowProps
{
	WindowProps(const int width = 1920, const int height = 1080, const std::string& title = "Raycaster") : Width(width), Height(height), Title(title) {}
	int Width;
	int Height;
	std::string Title;
};

class __declspec(dllexport) Window
{
public:
	Window(const WindowProps& props = WindowProps());
	virtual ~Window();

	SDL_Window* GetSDLPtr();
	void Resize(const int width, const int height);

	int GetWidth() const;
	int GetHeight() const;

private:
	std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> m_window;
	WindowProps m_props;
	SDL_GLContext m_context;
};

