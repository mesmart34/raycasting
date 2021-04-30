#include "Input.h"

std::array<bool, SDL_NUM_SCANCODES> Input::m_pressed;
std::array<bool, SDL_NUM_SCANCODES> Input::m_previousPressed;
std::array<bool, 10> Input::m_mousePressed;
std::array<bool, 10> Input::m_mousePreviousPressed;
int Input::mouseScreenX;
int Input::mouseScreenY;
vec2 Input::mouseAxis;
float Input::m_scale;
std::string* Input::m_inputString;
bool Input::m_inputMode;

void Input::SetCursorMode(CursorMode mode)
{
	if (mode == CursorMode::Hidden) 
		SDL_SetRelativeMouseMode(SDL_TRUE);
	else 
		SDL_SetRelativeMouseMode(SDL_FALSE);
}

vec2 Input::GetMousePosition()
{
	return vec2(mouseScreenX, mouseScreenY);
}

void Input::SetTextInputMode(bool enabled, std::string* const currentText)
{
	m_inputMode = enabled;
	m_inputString = currentText;
}


void Input::SetScale(const float value)
{
	m_scale = value;
}

bool Input::IsKeyDown(const int key)
{
	return m_pressed[key] && !m_previousPressed[key];
}

bool Input::IsKeyUp(const int key)
{
	return !m_pressed[key] && m_previousPressed[key];
}

bool Input::IsKeyPressed(const int key)
{
	return m_pressed[key] && m_previousPressed[key];
}

bool Input::IsMouseDown(const int btn)
{
	return m_mousePressed[btn] && !m_mousePreviousPressed[btn];
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
