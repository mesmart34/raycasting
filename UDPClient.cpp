#include "pch.h"
#include "UDPClient.h"

UDPClient::UDPClient()
	: m_wsaData{ 0 }, m_ipAddress(""), m_port(0), m_infoSize(sizeof(m_info)), m_socket(INVALID_SOCKET), m_info{ 0 }
{
	
}

UDPClient::~UDPClient()
{
	closesocket(m_socket);
	WSACleanup();
}

void UDPClient::Connect(const std::string& ip, const int port)
{

	m_info.sin_family = AF_INET;
	m_info.sin_port = htons(port);
	m_info.sin_addr.s_addr = inet_addr(ip.c_str());

	auto error = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (error)
		throw "WSA Error";
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_socket == SOCKET_ERROR)
		throw "WSA Error";
	bind(m_socket, (sockaddr*)&m_info, m_infoSize);


	u_long iMode = 1;
	ioctlsocket(m_socket, FIONBIO, &iMode);

	DoHandshake();
}

void UDPClient::Disconnect()
{
	m_buffer[0] = (char)ClientMessage::Leave;
	m_buffer[1] = m_id;
	std::cout << m_buffer[1] << std::endl;
	Send(&m_buffer[0], 2);
	Close();
}

void UDPClient::Send(const char* data, const int length)
{
	if ((sendto(m_socket, data, length, 0, (sockaddr*)&m_info, m_infoSize)) == SOCKET_ERROR)
	{
		//std::cout << WSAGetLastError() << std::endl;
	}
}

void UDPClient::Close()
{
	m_running = false;
	m_connected = false;
	WSACleanup();
	closesocket(m_socket);
	m_onDisconnectCallback(this);
}

void UDPClient::SetOnConnectCallback(std::function<void(UDPClient*)> callback)
{
	m_onConnectCallback = callback;
}

void UDPClient::SetOnDisconnectCallback(std::function<void(UDPClient*)> callback)
{
	m_onDisconnectCallback = callback;
}

void UDPClient::SetFailedToConnectCallback(std::function<void(UDPClient*)> callback)
{
	m_failedToConnectCallback = callback;
}

void UDPClient::SetOtherPlayerConnectCallback(std::function<void(UDPClient*, int)> callback)
{
	m_otherPlayerConnect = callback;
}

void UDPClient::SetOtherPlayerDisconnectCallback(std::function<void(UDPClient*, int)> callback)
{
	m_otherPlayerDisconnect = callback;
}

void UDPClient::SetOnMessage(std::function<void(UDPClient*, char*, int, ServerMessage)> callback)
{
	m_onMessage = callback;
}

bool UDPClient::IsConnected() const
{
	return m_connected;
}

PlayerData* UDPClient::PollMessage()
{
	return m_clientMessages.next();
}

uint16_t UDPClient::GetID() const
{
	return m_id;
}

void UDPClient::Receive()
{
	while (true)
	{
		char dataBuffer[MAX_PACKET_SIZE];
		auto len = recvfrom(m_socket, m_buffer, MAX_PACKET_SIZE, 0, 0, 0);
		if (len == SOCKET_ERROR)
			break;
		auto type = m_buffer[0];
		if (type == (char)ServerMessage::JoinResult)
		{
			if (m_buffer[1] == 1)
			{
				m_id = m_buffer[2];
				m_onConnectCallback(this);
				m_connected = true;
			}
			else {
				m_failedToConnectCallback(this);
			}
		}
		else if (type == (char)ServerMessage::PlayersState)
		{
			auto bytes = 1;
			while (bytes < len)
			{
				memcpy(&dataBuffer[0], &m_buffer[bytes], sizeof(PlayerData));
				m_onMessage(this, &dataBuffer[0], sizeof(PlayerData), (ServerMessage)type);
				bytes += sizeof(PlayerData);
			}
		}
		else if (m_buffer[0] == (char)ServerMessage::Door)
		{
			auto bytes = 1;
			while (bytes < len)
			{
				memcpy(&dataBuffer[0], &m_buffer[bytes], sizeof(DoorInfo));
				m_onMessage(this, &dataBuffer[0], sizeof(DoorInfo), (ServerMessage)type);
				bytes += sizeof(DoorInfo);
			}
		}
		else if (m_buffer[0] == (char)ServerMessage::ClientDisconnect)
		{
			auto id = 0;
			memcpy(&id, &m_buffer[1], sizeof(uint16_t));
			m_otherPlayerDisconnect(this, id);
		}
	}
}

void UDPClient::DoHandshake()
{
	memset(&m_buffer[0], 0, MAX_PACKET_SIZE);
	m_buffer[0] = (char)ClientMessage::Join;
	Send(&m_buffer[0], 1);
}
