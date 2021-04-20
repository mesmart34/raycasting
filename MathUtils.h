#pragma once
#include "vec2.h"
#include <cmath>
#include <cstdint>
#include "SDL/SDL.h"

class MathUtils
{
public:
	static vec2 Rotate(const vec2& other, const float angle);
	static float DegToRad(const float deg);
	static uint32_t PackRGBA(const SDL_Color color);
};