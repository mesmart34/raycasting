#pragma once

#include "vec2.h"
#include "MathUtils.h"
#include "Input.h"
#include "Object.h"

class Player : public Object
{
public:
	Player(const vec2& position = vec2(), const float angle = 0, const vec2& plane = vec2(1, 0));

	void Move(const vec2& other);
	void Rotate(const float angle);

	vec2 GetDirection() const;
	vec2 GetPlane() const;


	virtual void Update(const float deltaTime) final;
	
	
private:
	vec2 m_plane;
	vec2 m_direction;
	float m_fieldOfView;
};