#pragma once
#include <cstdint>
#include <string>


struct ClientMessage
{
	uint32_t Id;
	std::string Message;
};