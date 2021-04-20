#pragma once
#include "SDL/SDL.h"
#include "Event.h"
#include <iostream>
#include <array>
#include "vec2.h"

class Input
{
public:
	
	friend class EventHandler;

	static bool IsKeyDown(const int key);
	static bool IsKeyUp(const int key);
	static bool IsKeyPressed(const int key);
	static vec2 GetMouseAxis();

private:
	static void SeyKeyDown(const int key);
	static void SeyKeyUp(const int key);
	static void SetMouseScreenCoordinates(const int x, const int y);
	static void SetMouseAxis(const float x, const float y);

private:
	static std::array<bool, SDL_NUM_SCANCODES> m_pressed;
	static std::array<bool, SDL_NUM_SCANCODES> m_previousPressed;
	static int mouseScreenX, mouseScreenY;
	static vec2 mouseAxis;
};

