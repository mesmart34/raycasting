#include "Game.h"

int main(int argc, char** argv)
{
	auto game = new Game(1360, 768);
	game->Run();
	return 0;
}