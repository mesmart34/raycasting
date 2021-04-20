#pragma once
class Ray
{
public:
	Ray(const float distance, const bool horizontal, const int id, const float wallX) : m_distance(distance), m_horizonal(horizontal), m_id(id), m_wallX(wallX)
	{

	}

	float GetDistance() const;
	int GetID() const;
	bool IsHorizontal() const;
	float GetWallX() const;

private:
	
	float m_distance;
	bool m_horizonal;
	int m_id;
	float m_wallX;
};

