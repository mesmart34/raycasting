#include "UDPServer.h"
#include <thread>
#include "Map.h"

#undef main

int main()
{
	auto server = new UDPServer();
	auto serverThread = std::thread(&UDPServer::Start, server, 8888);
	serverThread.detach();
	std::string line;

	do
	{
		std::cout << "Waiting for a command..." << std::endl;
		std::cin >> line;
	} while (line != "stop");
	getchar();
	return 0;
}