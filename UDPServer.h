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
#include "ClientMessage.h"
#pragma comment(lib, "ws2_32.lib")

#define MAX_PACKET_SIZE 512

struct Client
{
	std::string IpAddress;
	int Port;
	uint32_t Id;
	std::chrono::time_point<std::chrono::high_resolution_clock> LastTime;
};

//struct ClientMessage
//{
//	uint32_t Id;
//	std::string Message;
//};

class UDPServer
{
public:
	UDPServer(const std::string& ip, const int port);

	void Start();

private:
	void Run();
	void Receive();
	void Send(const uint32_t id, const char* data, const int len);
	void DoHandshake(const uint32_t id);
	void CheckForDisconnects();
	//void AddClient();
	uint32_t GenerateUUID() const;

private:
	WSAData m_wsaData;
	std::string m_ipAddress;
	int m_port;
	SOCKET m_socket;
	sockaddr_in m_info;
	int m_infoSize;
	char m_buffer[MAX_PACKET_SIZE];
	SafeQueue<ClientMessage> m_messageQueue;
	std::thread m_receiveThread;
	std::map<uint32_t, Client> m_clients;
};

