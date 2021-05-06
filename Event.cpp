#include "pch.h"
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
		case SDL_MOUSEBUTTONDOWN:
		{
			auto btn = event.button.button;
			if (btn == SDL_BUTTON_LEFT)
			{
				auto btn = event.button.button;
				Input::m_mousePressed[btn] = true;
			}
			
		} break;
		case SDL_KEYDOWN:
		{
			if (Input::m_inputString == nullptr)
				break;
			if (event.key.keysym.sym == SDLK_BACKSPACE && Input::m_inputString->length() > 0)
			{
				Input::m_inputString->pop_back();
			}
			else if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
			{
				SDL_SetClipboardText(Input::m_inputString->c_str());
			}
			//Handle paste
			else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
			{
				Input::m_inputString->append(SDL_GetClipboardText());
			}
		} break;
		case SDL_MOUSEBUTTONUP:
		{
			auto btn = event.button.button;
			Input::m_mousePressed[btn] = false;
		} break;
		case SDL_TEXTINPUT:
		{
			if (!(SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V')))
			{
				if (Input::m_inputString != nullptr)
					Input::m_inputString->append(event.text.text);
			}
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
	for (auto i = 0; i < 3; i++)
		Input::m_mousePreviousPressed[i] = Input::m_mousePressed[i];
}
