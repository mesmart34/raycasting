#include "Event.h"

EventHandler::EventHandler()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void EventHandler::Handle()
{
	auto event = SDL_Event();
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
		{
			auto key = (int)event.key.keysym.scancode;
			Input::SeyKeyDown(key);
		} break;
		case SDL_KEYUP:
		{
			auto key = (int)event.key.keysym.scancode;
			Input::SeyKeyUp(key);
		} break;
		case SDL_MOUSEMOTION:
		{
			Input::SetMouseAxis(event.motion.xrel, event.motion.yrel);
			Input::SetMouseScreenCoordinates(event.motion.x, event.motion.y);
		} break;
		default:
			break;
		}
	}
}

void EventHandler::PostUpdate()
{
	Input::SetMouseAxis(0, 0);
}
