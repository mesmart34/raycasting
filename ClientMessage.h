#pragma once
#include <cstdint>
#include <string>
#include "Player.h"

struct PlayerInfo
{
	vec2 Position;
	float Angle;
};

struct ClientMessage
{
	uint32_t Id;
	std::string Message;
};