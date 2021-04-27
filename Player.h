#pragma once

#include "vec2.h"
#include "MathUtils.h"
#include "Input.h"

class Player
{
public:
	Player(const vec2& position = vec2(), const float angle = 0, const vec2& plane = vec2(1, 0));

	void Move(const vec2& other);
	void Rotate(const float angle);

	vec2 GetPosition() const;
	vec2 GetDirection() const;
	vec2 GetPlane() const;
	float GetRadius() const;
	float GetAngle() const;


	void Update(const float deltaTime);
	void SetPosition(const vec2& position);
	vec2 GetVelocity() const;
	void AddVelocity(const vec2& vel);
private:
	vec2 m_plane;
	vec2 m_direction;
	vec2 m_position;
	vec2 m_velocity;
	float m_radius;
	float m_angle;
	float m_fieldOfView;
};