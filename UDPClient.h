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
#include <chrono>
#include <functional>
#include <atomic>
#pragma comment(lib, "ws2_32.lib")

#define MAX_PACKET_SIZE 512
#define MAX_CLIENTS 4

class __declspec(dllexport) UDPClient
{
public:
	UDPClient(UDPClient&&) {};
	UDPClient();
	virtual ~UDPClient();

	void Connect(const std::string& ip, const int port);
	void Disconnect();
	void Send(const char* data, const int length);
	void Close();
	void SetOnConnectCallback(std::function<void(UDPClient*)>);
	void SetOnDisconnectCallback(std::function<void(UDPClient*)>);
	void SetFailedToConnectCallback(std::function<void(UDPClient*)>);
	void SetOtherPlayerConnectCallback(std::function<void(UDPClient*,int)>);
	void SetOtherPlayerDisconnectCallback(std::function<void(UDPClient*, int)>);
	void SetOnMessage(std::function<void(UDPClient*,char*, int, ServerMessage)>);
	bool IsConnected() const;

	uint16_t GetID() const;
	void Receive();

private:
	void DoHandshake();


private:
	WSAData m_wsaData;
	std::string m_ipAddress;
	int m_port;
	SOCKET m_socket;
	sockaddr_in m_info;
	int m_infoSize;
	char m_buffer[MAX_PACKET_SIZE];
	uint16_t m_id;
	SafeQueue<PlayerData> m_clientMessages;
	std::thread m_receiveThread;
	std::thread m_runThread;
	std::chrono::high_resolution_clock::time_point m_lastPacketTime;
	bool m_connected;
	std::function<void(UDPClient*)> m_onConnectCallback;
	std::function<void(UDPClient*)> m_onDisconnectCallback;
	std::function<void(UDPClient*)> m_failedToConnectCallback;
	std::function<void(UDPClient*, int)> m_otherPlayerConnect;
	std::function<void(UDPClient*, int)> m_otherPlayerDisconnect;
	std::function<void(UDPClient*, char*, int, ServerMessage)> m_onMessage;
	std::atomic<bool> m_running;

};

