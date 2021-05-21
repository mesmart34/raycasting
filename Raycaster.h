#pragma once
#include "Player.h"
#include <vector>
#include <iostream>
#include <map>
#include <iostream>
#include "Map.h"

struct Ray
{
	float Distance;
	bool Horizontal;
	int Id;
	float WallX;
	bool Door;
	bool DoorBox;
	float TextureOffset;
	vec2 Position;
};

struct Cast
{
	float CameraX = 0.0f;
	vec2 Direction = vec2();
	vec2 SideDistance = vec2();
	vec2 Step = vec2();
	vec2 Rounded = vec2();
	vec2 DeltaDistance = vec2();
	bool Vertical = false;
	bool Hit = false;
	float Distance = 0.0f;
	float WallX = 0.0f;
	int Id = 0;
	bool Door = false;
	bool DoorBox = false;
	float TextureOffset = 0.0f;
};

class Raycaster
{
public:
	Raycaster() = default;

	Ray CastRay(
		const int strip, 
		const int width, 
		const Player& player,
		Map* map) const;

private:
	void MakeStep(Cast& cast) const;
	float CalculateDistance(const Cast& cast, const Player& player) const;
	float CalculateWallX(const Cast& cast, const Player& player) const;
	void ProcessVerticalDoor(Cast& cast, const Player& player, Map* map) const;
	void ProcessHorizontalDoor(Cast& cast, const Player& player, Map* map) const;

};

