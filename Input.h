#pragma once
#include "SDL/SDL.h"
#include "Event.h"
#include <iostream>
#include <array>
#include "Event.h"
#include "vec2.h"


enum class CursorMode
{
	Show, Hidden
};


class Input
{
public:
	
	friend class EventHandler;

	static void SetScale(const float value);
	static bool IsKeyDown(const int key);
	static bool IsKeyUp(const int key);
	static bool IsKeyPressed(const int key);
	static bool IsMouseDown(const int btn);
	static vec2 GetMouseAxis();
	static void SetCursorMode(CursorMode mode);
	static vec2 GetMousePosition();
	static void SetTextInputMode(bool enabled, std::string* const currentText);

private:
	static void SeyKeyDown(const int key);
	static void SeyKeyUp(const int key);
	static void SetMouseScreenCoordinates(const int x, const int y);
	static void SetMouseAxis(const float x, const float y);

private:
	static std::array<bool, SDL_NUM_SCANCODES> m_pressed;
	static std::array<bool, SDL_NUM_SCANCODES> m_previousPressed;
	static std::array<bool, 10> m_mousePressed;
	static std::array<bool, 10> m_mousePreviousPressed;
	static std::string* m_inputString;
	static int mouseScreenX, mouseScreenY;
	static vec2 mouseAxis;
	static float m_scale;
	static bool m_inputMode;


};

