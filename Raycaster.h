#pragma once
#include "Player.h"
#include <vector>
#include <iostream>
#include <map>
#include <iostream>

struct Ray
{
	float Distance;
	bool Horizontal;
	int Id;
	float WallX;
	bool Door;
	bool DoorBox;
	float TextureOffset;
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
		const std::vector<std::vector<int>>& map, 
		const Player& player,
		const std::map<int, float> doors) const;

private:
	void MakeStep(Cast& cast) const;
	float CalculateDistance(const Cast& cast, const Player& player) const;
	float CalculateWallX(const Cast& cast, const Player& player) const;
	void ProcessVerticalDoor(Cast& cast, const Player& player, const std::map<int, float> doors, const int mapWidth) const;
	void ProcessHorizontalDoor(Cast& cast, const Player& player, const std::map<int, float> doors, const int mapWidth) const;

};

