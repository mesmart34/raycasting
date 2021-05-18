#include "pch.h"
#include "UDPServer.h"

using namespace std;
using namespace std::chrono_literals;

UDPServer::UDPServer()
	: m_wsaData{ 0 }, m_ipAddress(""), m_port(0), m_infoSize(sizeof(m_info)), m_socket(INVALID_SOCKET), m_info{ 0 }
{
	
}

void UDPServer::Start(const std::string& ip, const int port)
{
	m_port = port;
	m_ipAddress = ip;
	m_info.sin_family = AF_INET;
	m_info.sin_port = htons(port);
	m_info.sin_addr.s_addr = inet_addr(ip.c_str());

	WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	bind(m_socket, (sockaddr*)&m_info, m_infoSize);
	cout << "Server started at: " << inet_ntoa(m_info.sin_addr) << ":" << htons(m_info.sin_port) << endl;
	m_receiveThread = thread(&UDPServer::Receive, this);
	m_receiveThread.detach();
	m_runThread = thread(&UDPServer::Run, this);
	m_runThread.detach();
}

void UDPServer::Run()
{
	while (true)
	{
		CheckForDisconnects();
		//std::cout << m_clients.size() << std::endl;
		while (auto next = m_messageQueue.next())
		{
			auto& client = m_clients[next->Id];
			auto time = std::chrono::high_resolution_clock::now();
			client.LastTime = time;
			//std::cout << next->Id << ": " << next->Message << std::endl;
			for (auto c : m_clients)
			{
				//std::cout << c.first << std::endl;
				Send(c.first, (char*)next, sizeof(ClientMessage));
			}
		}
	}
}

void UDPServer::Receive()
{
	while (true)
	{
		memset(m_buffer, '\0', MAX_PACKET_SIZE);
		auto length = recvfrom(m_socket, m_buffer, MAX_PACKET_SIZE, 0, (sockaddr*)&m_info, &m_infoSize);
		if (length <= 0)
			continue;
		auto message = (ClientMessage*)(m_buffer);
		if (message->Type == MessageType::HEARTBEAT)
		{
			auto& client = m_clients[message->Id];
			client.LastTime = std::chrono::high_resolution_clock::now();
		}
		else if (message->Type == MessageType::HELLO)
		{
			auto client = Client();
			client.Id = GenerateUUID();
			client.IpAddress = string(inet_ntoa(m_info.sin_addr));
			client.Port = htons(m_info.sin_port);
			client.LastTime = std::chrono::high_resolution_clock::now();
			m_clients[client.Id] = client;
			std::cout << "Client " << client.Id << " has connected " << client.IpAddress << ":" << client.Port << std::endl;
			DoHandshake(client.Id);

			for (auto c : m_clients)
			{
				//std::cout << c.first << std::endl;
				auto clientMsg = ClientMessage();
				clientMsg.Id = client.Id;
				clientMsg.Type = MessageType::ON_PLAYER_CONNECT;
				Send(c.first, (char*)&clientMsg, sizeof(clientMsg));
			}

		}
		else {
			auto clientMessage = *(ClientMessage*)m_buffer;
			m_messageQueue.push(&clientMessage);
		}
	}
}

void UDPServer::Send(const uint32_t id, const char* data, const int len)
{
	auto destination = sockaddr_in();
	auto ip = m_clients[id].IpAddress;
	auto port = m_clients[id].Port;
	destination.sin_family = AF_INET;
	destination.sin_addr.s_addr = inet_addr(ip.c_str());
	destination.sin_port = htons(port);
	sendto(m_socket, data, len, 0, (sockaddr*)&destination, m_infoSize);
}

void UDPServer::DoHandshake(const uint32_t id)
{
	auto msg = ClientMessage();
	msg.Type = MessageType::HELLO;
	msg.Id = id;
	auto destination = sockaddr_in();
	auto ip = m_clients[id].IpAddress;
	auto port = m_clients[id].Port;
	destination.sin_port = htons(port);
	destination.sin_addr.s_addr = inet_addr(ip.c_str());
	destination.sin_family = AF_INET;
	if (sendto(m_socket, (char*)&msg, sizeof(ClientMessage), 0, (sockaddr*)&destination, m_infoSize) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError() << std::endl;
	}
}

void UDPServer::CheckForDisconnects()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	for (auto& client : m_clients)
	{
		auto time = client.second.LastTime;
		auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - time);
		auto id = client.second.Id;
		if (delta.count() > 5000)
		{
			m_clients.erase(id);
			std::cout << "Client " << id << " is disconnected!" << std::endl;
			for (auto c : m_clients)
			{
				//std::cout << c.first << std::endl;
				auto clientMsg = ClientMessage();
				clientMsg.Id = id;
				clientMsg.Type = MessageType::ON_PLAYER_DISCONNECT;
				Send(c.first, (char*)&clientMsg, sizeof(ClientMessage));
			}
		}
	}
}

uint32_t UDPServer::GenerateUUID() const
{
	auto id = (uint32_t)(rand());
	return id;
}
