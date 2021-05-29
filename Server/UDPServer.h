#pragma once
#include "Utils.h"
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
#include "ClientMessage.h"
#include "TRexStein/tinyxml2.h"
#include "StringUtils.h"
#include "Map.h"
#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 512
#define MAX_CLIENTS 4


class UDPServer
{
public:
	UDPServer();

	void Start(const int port);

private:
	void Run();
	void InitGameWorld();
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
	Scope<Map> m_map;
	std::map<int, Ref<NetPlayer>> m_players;
};

