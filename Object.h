#pragma once

#include "Sprite.h"
#include "vec2.h"
#include "MathUtils.h"

enum class ObjectType
{
	OBJECT, ENEMY, NET_PLAYER
};

class Object
{
public:
	Object() = default;
	Object(const Sprite& sprite, const vec2& position, const bool isCollidable);

	virtual ~Object() = default;

	Sprite GetSprite() const;
	void SetPosition(const vec2& position);
	vec2 GetPosition() const;
	vec2 GetVelocity() const;
	void SetVelocity(const vec2 velocity);
	bool IsCollidable() const;
	float GetRadius() const;
	float GetAngle() const;

	void AddVelocity(const vec2& vel);

	bool IsEnabled() const;
	void SetEnable(const bool value);
	ObjectType GetType() const;

	virtual void Update(const float deltaTime) {};
	virtual void Physics(const float deltaTime);
	virtual void CalculateSprite(const vec2& position) {};
	virtual void OnRaycastHit(int damage) { }

protected:
	Sprite m_sprite;
	vec2 m_position;
	vec2 m_velocity;
	float m_radius;
	float m_angle;
	bool m_isCollidable;
	bool m_isEnabled;
	ObjectType m_type;
};