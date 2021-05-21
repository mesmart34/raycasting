#pragma once
#include <cstdint>
#include <string>
#include "Player.h"
#include "Enemy.h"


struct IPEndPoint
{
	uint32_t IpAddress = 0;
	uint16_t Port = 0;
};

struct PlayerData
{
	uint16_t Id;
	vec2 Position;
	vec2 Velocity;
	float Angle;
	EnemyState State;
};

struct Client
{
	IPEndPoint EndPoint = {};
	std::chrono::time_point<std::chrono::high_resolution_clock> HeardTime = {};
	float Angle = 0.0;
	vec2 Position = vec2();
	vec2 Velocity = vec2();
	EnemyState State;
};

struct DoorInfo
{
	int x, y;
};

enum class ClientMessage : int8_t
{
	Join, Leave, Input, Door
};

enum class ServerMessage : int8_t
{
	JoinResult, PlayersState, ClientConnect, ClientDisconnect, Door
};
