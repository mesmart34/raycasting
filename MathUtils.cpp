#include "MathUtils.h"

#define M_PI 3.1415926535897

vec2 MathUtils::Rotate(const vec2& other, const float angle)
{
	return vec2(
		other.x * cosf(DegToRad(angle)) - other.y * sinf(DegToRad(angle)),
		other.x * sinf(DegToRad(angle)) + other.y * cosf(DegToRad(angle))
	);
}

float MathUtils::DegToRad(const float deg)
{
	return deg * M_PI / 180.0f;
}

uint32_t MathUtils::PackRGBA(const SDL_Color color)
{
	return (uint32_t)(color.r << 0 | color.g << 8 | color.b << 16 | color.a << 24);
}