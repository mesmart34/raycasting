#include "pch.h"
#include "RaycastEngine.h"

int main(int argc, char** argv)
{
	auto engine = new RaycastEngine(800, 600);
	engine->Run();
	return 0;
}