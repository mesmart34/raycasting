#pragma once
#include "Enemy.h"
class __declspec(dllexport) NetPlayer : public Enemy
{
public:
	NetPlayer();
	NetPlayer(const int clientId, const vec2& position, const float angle, const Sprite& sprite);

	int GetClientID() const;

	ObjectState GetState() const;
	void SetState(const ObjectState state);

	virtual void Update(const float deltaTime) final;
	virtual void CalculateSprite(const vec2& position) final;
	virtual void OnRaycastHit(int damage) final;

private:
	int m_clientID;
};

