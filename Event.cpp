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
		case SDL_MOUSEMOTION:
		{
			Input::SetMouseAxis(event.motion.xrel, event.motion.yrel);
			Input::SetMouseScreenCoordinates(event.motion.x, event.motion.y);
		} break;
		default:
			break;
		}
	}
	for (auto i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		Input::m_previousPressed[i] = Input::m_pressed[i];
		Input::m_pressed[i] = m_keyboard[i];
	}
}

void EventHandler::PostUpdate()
{
	Input::SetMouseAxis(0, 0);
}

