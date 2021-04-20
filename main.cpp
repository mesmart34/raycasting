#include "Game.h"



int main(int argc, char** argv)
{
	auto game = new Game(304 * 2, 152 * 2);
	game->Run();
	return 0;
}