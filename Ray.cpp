#include "Ray.h"

float Ray::GetDistance() const
{
	return m_distance;
}

int Ray::GetID() const
{
	return m_id;
}

bool Ray::IsHorizontal() const
{
	return m_horizonal;
}

float Ray::GetWallX() const
{
	return m_wallX;
}
