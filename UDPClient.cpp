#include "pch.h"
#include "UDPClient.h"

UDPClient::UDPClient(const std::string& ip, const int port)
	: m_wsaData{ 0 }, m_ipAddress(ip), m_port(port), m_infoSize(sizeof(m_info)), m_socket(INVALID_SOCKET), m_info{ 0 }
{
	m_info.sin_family = AF_INET;
	m_info.sin_port = htons(m_port);
	m_info.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

	auto error = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (error)
		throw "WSA Error";
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_socket == SOCKET_ERROR)
		throw "WSA Error";
	std::cout << "Connected to " << inet_ntoa(m_info.sin_addr) << ":" << htons(m_info.sin_port) << std::endl;
}

UDPClient::~UDPClient()
{
	closesocket(m_socket);
	WSACleanup();
}

void UDPClient::Connect()
{
	DoHandshake();
	//Receive();
	std::cout << "I connected as " << m_id << std::endl;
	m_receiveThread = std::thread(&UDPClient::Receive, this);
	m_receiveThread.detach();
	m_runThread = std::thread(&UDPClient::Run, this);
	m_runThread.detach();
}

void UDPClient::Run()
{
	int a = 0;
	while (true)
	{
		a++;
		while (auto msg = m_clientMessages.next())
		{
			std::cout << msg->Id << ": " << msg->Message << std::endl;
		}
		auto clientMessage = ClientMessage();
		clientMessage.Id = m_id;
		clientMessage.Message = std::to_string(a);
		auto msg = (char*)&clientMessage;
		Send(msg, sizeof(clientMessage));
		Sleep(1);

	}
}

void UDPClient::Send(const char* data, const int length)
{
	if ((sendto(m_socket, data, length, 0, (sockaddr*)&m_info, m_infoSize)) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError() << std::endl;
	}
}

void UDPClient::Receive()
{
	while (true) {
		auto length = recvfrom(m_socket, m_buffer, MAX_PACKET_SIZE, 0, (sockaddr*)&m_info, &m_infoSize);
		if (length <= 0)
			continue;
		auto message = (ClientMessage*)(m_buffer);
		std::cout << message->Id << ": " << message->Message << std::endl;
		m_clientMessages.push(message);
	}
}

void UDPClient::DoHandshake()
{
	auto message = std::string("hello");
	if ((sendto(m_socket, message.c_str(), message.size(), 0, (sockaddr*)&m_info, m_infoSize)) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError() << std::endl;
	}
	auto length = recvfrom(m_socket, m_buffer, MAX_PACKET_SIZE, 0, (sockaddr*)&m_info, &m_infoSize);
	auto id = atol(m_buffer);
	m_id = id;
}
