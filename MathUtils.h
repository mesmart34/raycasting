#pragma once
#include "vec2.h"
#include <cmath>
#include <cstdint>
#include "SDL/SDL.h"

class __declspec(dllexport) MathUtils
{
public:
	static vec2 Rotate(const vec2& other, const float angle);
	static float DegToRad(const float deg);
	static float RadToDeg(const float rad);
	static uint32_t PackRGBA(const SDL_Color color);
	static int GetAngularDifference(const int a1, const int a2);
	static int Modulo(const int a, const int n);
	static float Clamp(const float target, const float min, const float max);
	static float Lerp(const float a, const float b, const float t);
};