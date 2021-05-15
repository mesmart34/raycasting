#pragma once
#include "SafeQueue.h"
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>
#include <cassert>
#include <iostream>
#include <string>
#include <cstdint>
#include "ClientMessage.h"
#include <thread>
#pragma comment(lib, "ws2_32.lib")

#define MAX_PACKET_SIZE 512


class UDPClient
{
public:
	UDPClient(UDPClient&&) {};
	UDPClient(const std::string& ip, const int port);
	~UDPClient();

	void Connect();
	void Send(const char* data, const int length);

private:
	void Run();
	void Receive();
	void DoHandshake();


private:
	WSAData m_wsaData;
	std::string m_ipAddress;
	int m_port;
	SOCKET m_socket;
	sockaddr_in m_info;
	int m_infoSize;
	char m_buffer[MAX_PACKET_SIZE];
	uint32_t m_id;
	SafeQueue<ClientMessage> m_clientMessages;
	std::thread m_receiveThread;
	std::thread m_runThread;
};

