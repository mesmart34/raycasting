#pragma once
#include "SDL/SDL.h"
#include "Input.h"
#include <array>
#include "imgui_impl_sdl.h"

class __declspec(dllexport) EventHandler
{
public:
	EventHandler();
	void Handle();
	void PostUpdate();

	friend class Input;

private:
	const Uint8* m_keyboard = SDL_GetKeyboardState(NULL);
};

