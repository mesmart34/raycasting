#pragma once

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>
#include <cassert>
#include <iostream>
#include <thread>
#include <map>
#include "SafeQueue.h"
#include <cstdlib>
#include <chrono>
#include <cstdint>
#include "vec2.h"
#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 512
#define MAX_CLIENTS 4

enum class EnemyState
{
	Idle, Walk, Attack, Die, Damaged
};


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
	std::chrono::time_point<std::chrono::high_resolution_clock> HeardTime;
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

class UDPServer
{
public:
	UDPServer();

	void Start(const int port);

	void Run();
	void Receive();
	void UpdatePlayerData();
	void UpdateWorld(const float deltaTime);
	void SendWorldData();
	void AddClient(sockaddr_in& from);
	void RemoveClient(const IPEndPoint& endPoint);
	void CheckForDisconnect(const uint16_t id);
	void SendDisconnectMessage(const int id);
	void SendToClient(const Client& client, const char* buffer, const int len);

private:
	WSAData m_wsaData;
	SOCKET m_socket;
	Client m_clients[MAX_CLIENTS];
	sockaddr_in m_addressInfo;
	IPEndPoint m_endPoint;
	bool m_running;
	char m_buffer[MAX_BUFFER_SIZE];
	std::queue<DoorInfo> m_doorsToOpen;
};

