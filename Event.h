#pragma once

#include "SDL/SDL.h"
#include "Input.h"
#include <array>


class EventHandler
{
public:
	EventHandler();
	void Handle();
	void PostUpdate();

	


private:
	const Uint8* m_keyboard = SDL_GetKeyboardState(NULL);

};

