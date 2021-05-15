#include "pch.h"
#include "RaycastEngine.h"

int main(int argc, char** argv)
{
	auto engine = new RaycastEngine(1360, 768);
	engine->Run();
	return 0;
}