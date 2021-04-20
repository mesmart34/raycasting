#include "Game.h"



int main(int argc, char** argv)
{
	auto game = new Game(304, 152);
	game->Run();
	return 0;
}