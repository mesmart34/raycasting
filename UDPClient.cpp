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

	//bind(m_socket, (sockaddr*)&m_info, m_infoSize);
	//std::cout << "wdad" << std::endl;
	DoHandshake();
	m_receiveThread = std::thread(&UDPClient::Receive, this);
	m_receiveThread.detach();
	m_runThread = std::thread(&UDPClient::Run, this);
	m_runThread.detach();
	m_running = true;
}

void UDPClient::Run()
{
	while (m_running)
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_lastPacketTime);
		if (delta.count() > 5000)
		{
			if (!m_connected)
			{
				//m_failedToConnectCallback(this);
				m_running = false;
				m_failedToConnectCallback(this);
			}
			else {
				m_connected = false;
				Close();
			}
		}
	}
}

void UDPClient::Send(const char* data, const int length)
{
	if ((sendto(m_socket, data, length, 0, (sockaddr*)&m_info, m_infoSize)) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError() << std::endl;
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

ClientMessage UDPClient::BuildMessage(const char* data) const
{
	auto clientMessage = ClientMessage();
	clientMessage.Id = m_id;
	clientMessage.Message = data;
	return clientMessage;
}

bool UDPClient::IsConnected() const
{
	return m_connected;
}

ClientMessage* UDPClient::PollMessage()
{
	return m_clientMessages.next();
}

int UDPClient::GetID() const
{
	return m_id;
}

void UDPClient::Receive()
{
	while (m_running) {
		auto length = recvfrom(m_socket, m_buffer, MAX_PACKET_SIZE, 0, (sockaddr*)&m_info, &m_infoSize);
		m_lastPacketTime = std::chrono::high_resolution_clock::now();
		if(m_connected)
		{
			auto message = (ClientMessage*)(m_buffer);
			//std::cout << message->Id << ": " << message->Message << std::endl;
			m_clientMessages.push(message);
		}
		else {
			auto id = atol(m_buffer);
			m_id = id;
			m_connected = true;
			m_onConnectCallback(this);
		}
	}
}

void UDPClient::DoHandshake()
{
	auto message = std::string("hello");
	if ((sendto(m_socket, message.c_str(), message.size(), 0, (sockaddr*)&m_info, m_infoSize)) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError() << std::endl;
	}
	m_lastPacketTime = std::chrono::high_resolution_clock::now();
}
