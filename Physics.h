#pragma once
#include "vec2.h"

class Physics
{
public:
	static bool Intersection(const vec2& a, const vec2& b, const float a_size, const float b_size);
};