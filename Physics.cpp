#include "Physics.h"

bool Physics::Intersection(const vec2& a, const vec2& b, const float a_size, const float b_size)
{
	if (a.x < b.x + a_size &&
		a.x + a_size > b.x &&
		a.y < b.y + b_size &&
		a.y + b_size > b.y)
	{
		return true;
	}
	return false;
}