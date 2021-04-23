#include "Enemy.h"

Enemy::Enemy(const Sprite& sprite, const vec2& position, const float angle)
	: Object(sprite, position, true), m_angle(angle)
{

}

void Enemy::Update(const float deltaTime, const Player& player)
{
	m_angle -= deltaTime * 100;
	if (m_angle >= 360)
		m_angle -= 360;
	if (m_angle < 0)
		m_angle += 360;
	auto diff = (int)(MathUtils::GetAngularDifference(player.GetAngle(), GetAngle()) / 45.0f) + 8 * 5;
	//printf("%d, %d\n", m_angle, diff);
	m_sprite.Id = diff;
}

float Enemy::GetAngle() const
{
	return m_angle;
}
