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

float MathUtils::RadToDeg(const float rad)
{
	return  rad * 180.0f / M_PI;
}

uint32_t MathUtils::PackRGBA(const SDL_Color color)
{
	auto res = *(uint32_t*)&color;
	return res;
	//return (uint32_t)(color.a << 24 | color.r << 16 | color.g << 8 | color.b << 0);
}

int MathUtils::GetAngularDifference(const int a1, const int a2)
{
	auto result = Modulo((a1 - a2 + 180), 360) - 180;
	if (result < 0)
		result += 360;
	return result;
}

int MathUtils::Modulo(const int a, const int n)
{
	return a - floor(a / n) * n;
}

float MathUtils::Clamp(const float target, const float min, const float max)
{
	if (target < min)
		return min;
	if (target > max)
		return max;
	return target;
}

float MathUtils::Lerp(const float a, const float b, const float t)
{
	auto res = a + t * (b - a);
	return res;
}
