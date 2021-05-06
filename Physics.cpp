#include "pch.h"
#include "Physics.h"

vec2 Physics::Intersection(const float circleRadius, const vec2& circlePosition, const vec2& rectPosition, const vec2& rectSize)
{
    auto delta = vec2(
        circlePosition.x - fmax(rectPosition.x, fmin(circlePosition.x, rectPosition.x + rectSize.x)),
        circlePosition.y - fmax(rectPosition.y, fmin(circlePosition.y, rectPosition.y + rectSize.y))
    );
    auto distance = delta.abs() - (circleRadius);
    if (distance > 0)
        return vec2();
    auto x = MathUtils::Clamp(circlePosition.x, rectPosition.x, rectPosition.x + rectSize.x);
    auto y = MathUtils::Clamp(circlePosition.y, rectPosition.y, rectPosition.y + rectSize.y);

    auto dx = x - circlePosition.x;
    auto dy = y - circlePosition.y;
    auto dd = sqrt(dx * dx + dy * dy);
    auto csx = circlePosition.x + dx / dd * circleRadius;
    auto csy = circlePosition.y + dy / dd * circleRadius;
	return vec2(csx - x, csy - y);
}

vec2 Physics::Intersection(const float dynamicCircleRadius, const vec2& dynamicCirlcePos, const float staticCircleRadius, const vec2& staticCirlcePos)
{
    auto diff = dynamicCirlcePos - staticCirlcePos;
    auto abs = diff.abs();
    auto distance = abs - (dynamicCircleRadius + staticCircleRadius);
    if (distance > 0)
        return vec2();
    return diff * (distance / (dynamicCircleRadius + staticCircleRadius));
}
