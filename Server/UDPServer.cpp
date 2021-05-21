#include "UDPServer.h"

using namespace std;
using namespace std::chrono_literals;

typedef std::chrono::high_resolution_clock Timer;

UDPServer::UDPServer()
	: m_wsaData{ 0 }, m_addressInfo{ 0 }, m_socket(INVALID_SOCKET)
{

}

void UDPServer::Start(const int port)
{
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData))
		throw "WSA Startup Error!";

	m_endPoint = IPEndPoint();
	m_endPoint.IpAddress = inet_addr("127.0.0.1");
	m_endPoint.Port = port;

	auto localAddress = sockaddr_in();
	localAddress.sin_family = AF_INET;
	localAddress.sin_addr.s_addr = m_endPoint.IpAddress;
	localAddress.sin_port = htons(m_endPoint.Port);

	m_socket = socket(
		AF_INET,
		SOCK_DGRAM,
		IPPROTO_UDP
	);
	if (m_socket == INVALID_SOCKET)
		throw "Invalid socket!";

	

	auto bindError = bind(m_socket, (sockaddr*)&localAddress, sizeof(localAddress));

	/*if (bindError == SOCKET_ERROR)
		throw "Bind error!";*/


	u_long iMode = 1;
	ioctlsocket(m_socket, FIONBIO, &iMode);

	cout << "Server started at: " << inet_ntoa(localAddress.sin_addr) << ":" << htons(localAddress.sin_port) << endl;


	for (auto i = 0; i < MAX_CLIENTS; i++)
	{
		m_clients[i] = {};
	}

	Run();

}


void UDPServer::Run()
{
	auto startTime = Timer::now();
	m_running = true;
	while (m_running)
	{
		auto endTime = Timer::now();
		auto deltaTime = chrono::duration<float>(endTime - startTime).count();
		
		Receive();

		UpdateWorld(deltaTime);

		SendWorldData();



		Sleep(16);

	}
}

void UDPServer::Receive()
{
	
	while (true)
	{
		auto from = sockaddr_in();
		int fromSize = sizeof(sockaddr_in);
		auto bytes = recvfrom(m_socket, m_buffer, MAX_BUFFER_SIZE, 0, (sockaddr*)&from, &fromSize);
		if (bytes == SOCKET_ERROR)
		{
			auto error = WSAGetLastError();
			/*if (error != WSAEWOULDBLOCK)
				cout << error << endl;*/
			break;
		}

		auto endPoint = IPEndPoint();
		endPoint.IpAddress = from.sin_addr.S_un.S_addr;
		endPoint.Port = from.sin_port;
		//std::cout << (int)m_buffer[0] << std::endl;
		switch (m_buffer[0])
		{
		case (char)ClientMessage::Join:
		{
			AddClient(from);
		} break;
		case (char)ClientMessage::Leave:
		{
			RemoveClient(endPoint);
		} break;
		case (char)ClientMessage::Input:
		{
			UpdatePlayerData();
		} break;
		case (char)ClientMessage::Door:
		{
			auto door = DoorInfo();
			memcpy(&door, &m_buffer[1], sizeof(DoorInfo));
			std::cout << "Door is opened at " << door.x << ", " << door.y << std::endl;
			m_doorsToOpen.push(door);
		} break;
		}
	}
}

void UDPServer::UpdatePlayerData()
{
	auto slot = m_buffer[1];

	auto& client = m_clients[slot];
	auto data = *(PlayerData*)&m_buffer[3];
	client.Position = data.Position;
	client.Velocity = data.Velocity;
	client.Angle = data.Angle;
	client.State = data.State;
	//std::cout << (int)slot << ": " << client.Angle << std::endl;
	client.HeardTime = Timer::now();
}

void UDPServer::UpdateWorld(const float deltaTime)
{
	for (auto i = 0; i < MAX_CLIENTS; i++)
	{
		auto &client = m_clients[i];

		if (!client.EndPoint.IpAddress)
			continue;
		//client.Velocity *= deltaTime;
		client.State = vec2::get_magnitude(client.Velocity) < 0.001 ? EnemyState::Idle : EnemyState::Walk;
		//client.Position += client.Velocity;
		CheckForDisconnect(i);
	}
}

void UDPServer::SendWorldData()
{
	m_buffer[0] = (char)ServerMessage::PlayersState;
	auto bytesWriten = sizeof(char);
	for (uint16_t i = 0; i < MAX_CLIENTS; i++)
	{
		auto& client = m_clients[i];
		if (client.EndPoint.IpAddress)
		{
			auto data = PlayerData();
			data.Id = i;
			data.Angle = client.Angle;
			data.Position = client.Position;
			data.Velocity = client.Velocity;
			data.State = client.State;
			
			auto size = sizeof(PlayerData);
			memcpy(&m_buffer[bytesWriten], (char*)&data, size);
			

			bytesWriten += size;
		}
	}


	for (uint16_t i = 0; i < MAX_CLIENTS; i++)
	{
		auto& client = m_clients[i];
		SendToClient(client, m_buffer, bytesWriten);
	}

	//Doors
	bytesWriten = 0;
	m_buffer[0] = (char)ServerMessage::Door;
	bytesWriten += sizeof(char);

	for (auto i = 0; i < m_doorsToOpen.size(); i++)
	{
		auto door = m_doorsToOpen.back();
		m_doorsToOpen.pop();
		memcpy(&m_buffer[bytesWriten], &door, sizeof(DoorInfo));
		bytesWriten += sizeof(DoorInfo);
	}

	for (uint16_t i = 0; i < MAX_CLIENTS; i++)
	{
		auto& client = m_clients[i];
		SendToClient(client, m_buffer, bytesWriten);
	}
}

void UDPServer::AddClient(sockaddr_in& from)
{
	auto slot = (uint16_t)(-1);

	for (auto i = 0; i < MAX_CLIENTS; i++)
	{
		auto& client = m_clients[i];
		if (!client.EndPoint.IpAddress)
		{
			slot = i;
			break;
		}
	}

	m_buffer[0] = (char)ServerMessage::JoinResult;
	//if there are available slots....
	if (slot != (uint16_t)(-1))
	{
		m_buffer[1] = 1;
		memcpy(&m_buffer[2], &slot, 2);
		auto error = sendto(m_socket, m_buffer, 4, 0, (sockaddr*)&from, sizeof(from));
		if (error != SOCKET_ERROR)
		{
			std::cout << "Client " << slot << " is connected" << std::endl;
			auto client = Client();
			client.EndPoint.IpAddress = from.sin_addr.S_un.S_addr;
			client.EndPoint.Port = from.sin_port;
			client.HeardTime = Timer::now();
			m_clients[slot] = client;

		}
		else
		{
			memset(&m_clients[slot], 0, sizeof(Client));
		}
	}
	// no slots...
	else {
		m_buffer[1] = 0;
		sendto(m_socket, m_buffer, 2, 0, (sockaddr*)&from, sizeof(from));
	}

}

void UDPServer::RemoveClient(const IPEndPoint& endPoint)
{
	auto slot = (uint16_t)(-1);
	slot = m_buffer[1];
	if (m_clients[slot].EndPoint.IpAddress == endPoint.IpAddress &&
		m_clients[slot].EndPoint.Port == endPoint.Port)
	{
		m_clients[slot] = {};
		SendDisconnectMessage(slot);
	}
}

void UDPServer::CheckForDisconnect(const uint16_t id)
{
	auto client = m_clients[id];
	auto time = Timer::now();
	auto delta = chrono::duration_cast<chrono::seconds>(time - client.HeardTime).count();
	if (delta > 5)
	{
		m_clients[id] = {};
		SendDisconnectMessage(id);
	}
}

void UDPServer::SendDisconnectMessage(const int id)
{
	std::cout << "Player " << id << " is disconnected" << std::endl;
	m_buffer[0] = (char)ServerMessage::ClientDisconnect;
	memcpy(&m_buffer[1], &id, sizeof(uint16_t));
	auto bytesWritten = 0;
	bytesWritten += sizeof(char);
	bytesWritten += sizeof(uint16_t);

	for (auto i = 0; i < MAX_CLIENTS; i++)
	{
		SendToClient(m_clients[i], &m_buffer[0], bytesWritten);
	}
}

void UDPServer::SendToClient(const Client& client, const char* m_buffer, const int len)
{
	if (!client.EndPoint.IpAddress)
		return;
	sockaddr_in to = {};
	auto toSize = sizeof(to);
	to.sin_family = AF_INET;
	to.sin_addr.S_un.S_addr = client.EndPoint.IpAddress;
	to.sin_port = client.EndPoint.Port;
	sendto(m_socket, m_buffer, len, 0, (sockaddr*)&to, toSize);
}
