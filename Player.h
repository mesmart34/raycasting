#pragma once

#include "vec2.h"
#include "MathUtils.h"

class Player
{
public:
	Player(const vec2& position = vec2(0, 0), const float angle = 0.0f);

	void Move(const vec2& other);
	void Rotate(const float angle);

	vec2 GetPosition() const;
	vec2 GetDirection() const;
	vec2 GetPlane() const;
	float GetRadius() const;
	float GetAngle() const;

	void SetPosition(const vec2& position);
private:
	vec2 m_plane;
	vec2 m_direction;
	vec2 m_position;
	float m_radius;
	float m_angle;
	float m_fieldOfView;
};