#pragma once
#include <cstdint>
#include <string>
#include "Player.h"

enum class MessageType
{
	ID,
	PLAYER_INFO,
	DOOR_STATE,
	ON_PLAYER_CONNECT,
	ON_PLAYER_DISCONNECT,
	RAY_CAST_EVENT,
	HEARTBEAT,
	HELLO
};

struct PlayerInfo
{
	vec2 Velocity;
	vec2 Position;
	float Angle;
};

struct ServerMessage
{
	MessageType Type;
	std::string Message;
};

struct ClientMessage
{
	MessageType Type;
	uint32_t Id;
	vec2 Velocity;
	vec2 Position;
	float Angle;
};