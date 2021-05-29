#pragma once

#include "Sprite.h"
#include "vec2.h"
#include "MathUtils.h"



enum class ObjectState
{
	Idle, Walk, Attack, Die, Damaged
};


class __declspec(dllexport) Object
{
public:
	Object() = default;
	Object(const vec2& position, const bool isCollidable, const Sprite& sprite);
	Object(const int id, const Sprite& sprite, const vec2& position, const bool isCollidable);

	virtual ~Object() = default;

	void SetID(const int id);
	void SetSprite(const Sprite& sprite);
	void SetPosition(const vec2& position);
	void SetAngle(const float angle);
	void SetVelocity(const vec2 velocity);
	void SetEnable(const bool value);
	void SetColliable(const bool value);
	float GetRadius() const;
	int GetID() const;
	float GetAngle() const;
	vec2 GetPosition() const;
	Sprite GetSprite() const;
	vec2 GetVelocity() const;
	void AddVelocity(const vec2& vel);
	bool IsCollidable() const;
	bool IsEnabled() const;


	virtual void Update(const float deltaTime) {};
	virtual void Physics(const float deltaTime);
	virtual void CalculateSprite(const vec2& position) {};
	virtual void OnRaycastHit(int damage) { }

protected:
	int m_id;
	Sprite m_sprite;
	vec2 m_position;
	vec2 m_velocity;
	float m_radius;
	float m_angle;
	bool m_isCollidable;
	bool m_isEnabled;
};