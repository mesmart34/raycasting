#pragma once
#include "vec2.h"
#include "MathUtils.h"
#include <iostream>

class __declspec(dllexport) Physics
{
public:
	static vec2 Intersection(const float circleRadius, const vec2& circlePosition, const vec2& rectPosition, const vec2& rectSize);
	static vec2 Intersection(const float cr1, const vec2& c1, const float cr2, const vec2& c2);
};