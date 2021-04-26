#include "Input.h"

std::array<bool, SDL_NUM_SCANCODES> Input::m_pressed;
std::array<bool, SDL_NUM_SCANCODES> Input::m_previousPressed;
int Input::mouseScreenX;
int Input::mouseScreenY;
vec2 Input::mouseAxis;
float Input::m_scale;

void Input::SetCursorMode(CursorMode mode)
{
	if (mode == CursorMode::Hidden) {
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	else {
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
}


void Input::SetScale(const float value)
{
	m_scale = value;
}

bool Input::IsKeyDown(const int key)
{
	//std::cout << !m_pressed[key] << ", " <<  m_previousPressed[key] << std::endl;
	return m_pressed[key] && !m_previousPressed[key];
}

bool Input::IsKeyUp(const int key)
{
	return !m_pressed[key] && m_previousPressed[key];
}

bool Input::IsKeyPressed(const int key)
{
	return m_pressed[key];
}

bool Input::IsMouseDown(const int btn)
{
	return false;
}

vec2 Input::GetMouseAxis()
{
	return mouseAxis * (1.0f / m_scale);
}

void Input::SeyKeyDown(const int key)
{
	m_previousPressed[key] = m_pressed[key];
	m_pressed[key] = true;
}

void Input::SeyKeyUp(const int key)
{
	m_previousPressed[key] = m_pressed[key];
	m_pressed[key] = false;
}

void Input::SetMouseScreenCoordinates(const int x, const int y)
{
	mouseScreenX = x;
	mouseScreenY = y;
}

void Input::SetMouseAxis(const float x, const float y)
{
	mouseAxis = vec2(x, y);
}
