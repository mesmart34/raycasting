#pragma once
#include <cstdint>
#include <string>
#include "vec2.h"

enum class EnemyState
{
	Idle, Walk, Attack, Die, Damaged
};

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
	EnemyState State;
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
	EnemyState State;
};