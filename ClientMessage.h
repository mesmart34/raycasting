#pragma once
#include <cstdint>
#include <string>
#include "Object.h"
#include "Player.h"


struct IPEndPoint
{
	uint32_t IpAddress = 0;
	uint16_t Port = 0;
};

struct PlayerData
{
	uint16_t ClientID;
	vec2 Position;
	vec2 Velocity;
	float Angle;
	ObjectState State;
};

struct ObjectData
{
	int Id;
	vec2 Position;
	vec2 Velocity;
	float Angle;
	ObjectState State;
};

struct Client
{
	int Id;
	IPEndPoint EndPoint = {};
	std::chrono::time_point<std::chrono::high_resolution_clock> HeardTime = {};
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
	JoinResult, PlayersState, ClientConnect, ClientDisconnect, Door, ObjectState
};
