#include "pch.h"
#include "UDPServer.h"

using namespace std;
using namespace std::chrono_literals;

UDPServer::UDPServer(const std::string& ip, const int port)
	: m_wsaData{0}, m_ipAddress(ip), m_port(port), m_infoSize(sizeof(m_info)), m_socket(INVALID_SOCKET), m_info{0}
{
	m_info.sin_family = AF_INET;
	m_info.sin_port = htons(m_port);
	m_info.sin_addr.s_addr = inet_addr(ip.c_str());
	
	WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	bind(m_socket, (sockaddr*)&m_info, m_infoSize);
	cout << "Server started at: " << inet_ntoa(m_info.sin_addr) << ":" << htons(m_info.sin_port) << endl;
}

void UDPServer::Start()
{
	m_receiveThread = thread(&UDPServer::Receive, this);
	m_receiveThread.detach();
	Run();
}

void UDPServer::Run()
{
	while (true)
	{
		CheckForDisconnects();
		while (auto next = m_messageQueue.next())
		{
			auto &client = m_clients[next->Id];
			auto time = std::chrono::high_resolution_clock::now();
			client.LastTime = time;
			for (auto c : m_clients)
			{
				if (c.first == next->Id)
					continue;
				Send(c.first, (char*)next,
					sizeof(ClientMessage));
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
		/*if (length == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			throw "WSA ERROR";
		}*/
		if (length <= 0)
			continue;
		auto message = string(m_buffer);
		if (message == "hello")	
		{
			auto client = Client();
			client.Id = GenerateUUID();
			client.IpAddress = string(inet_ntoa(m_info.sin_addr));
			client.Port = htons(m_info.sin_port);
			client.LastTime = std::chrono::high_resolution_clock::now();
			m_clients[client.Id] = client; 
			std::cout << "Client " << client.Id << " has connected" << std::endl;
			DoHandshake(client.Id);
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
	destination.sin_port = htons(port);
	destination.sin_addr.s_addr = inet_addr(ip.c_str());
	destination.sin_family = AF_INET;
	sendto(m_socket, data, len, 0, (sockaddr*)&m_info, m_infoSize);
}

void UDPServer::DoHandshake(const uint32_t id)
{
	auto msg = std::to_string(id);
	auto destination = sockaddr_in();
	auto ip = m_clients[id].IpAddress;
	auto port = m_clients[id].Port;
	destination.sin_port = htons(port);
	destination.sin_addr.s_addr = inet_addr(ip.c_str());
	destination.sin_family = AF_INET;
	sendto(m_socket, msg.c_str(), msg.size(), 0, (sockaddr*)&m_info, m_infoSize);
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
		}
	}
}

uint32_t UDPServer::GenerateUUID() const
{
	auto id = (uint32_t)(rand());
	return id;
}
