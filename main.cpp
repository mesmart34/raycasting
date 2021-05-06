#include "pch.h"
#include "RaycastEngine.h"

int main(int argc, char** argv)
{
	auto engine = new RaycastEngine(1920 / 2, 1080 / 2);
	engine->Run();
	return 0;
}