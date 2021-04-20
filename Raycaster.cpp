#include "Raycaster.h"

using namespace std;

Ray Raycaster::CastRay(const int strip, const int width, const vector<vector<int>>& map, const Player& player) const
{
	auto camera_x = 2 * strip / (float)width - 1;
	auto rayDir = vec2(
		player.GetDirection().x + player.GetPlane().x * camera_x,
		player.GetDirection().y + player.GetPlane().y * camera_x
	);
	auto deltaDist = vec2(
		(rayDir.y == 0) ? 0 : ((rayDir.x == 0) ? 1 : fabs(1 / rayDir.x)),
		(rayDir.x == 0) ? 0 : ((rayDir.y == 0) ? 1 : fabs(1 / rayDir.y))
	);
	auto step = vec2(0, 0);
	auto sideDist = vec2(0, 0);
	auto horizontal = false;
	auto roundedCoords = vec2((int)player.GetPosition().x, (int)player.GetPosition().y);
	if (rayDir.x < 0)
		sideDist.x = (player.GetPosition().x - roundedCoords.x) * deltaDist.x;
	else
		sideDist.x = (roundedCoords.x + 1.0f - player.GetPosition().x) * deltaDist.x;
	step.x = rayDir.x < 0 ? -1 : 1;

	if (rayDir.y < 0)
		sideDist.y = (player.GetPosition().y - roundedCoords.y) * deltaDist.y;
	else
		sideDist.y = (roundedCoords.y + 1.0f - player.GetPosition().y) * deltaDist.y;
	step.y = rayDir.y < 0 ? -1 : 1;

	while (true)
	{
		if (sideDist.x < sideDist.y)
		{
			sideDist.x += deltaDist.x;
			roundedCoords.x += step.x;
			horizontal = false;
		}
		else
		{
			sideDist.y += deltaDist.y;
			roundedCoords.y += step.y;
			horizontal = true;
		}
		if (map[(int)roundedCoords.x][(int)roundedCoords.y] > 0)
			break;
	}
	auto distance = 0.0f;
	if (horizontal == false)
		distance = (roundedCoords.x - player.GetPosition().x + (1 - step.x) / 2) / rayDir.x;
	else
		distance = (roundedCoords.y - player.GetPosition().y + (1 - step.y) / 2) / rayDir.y;
	auto wallX = 0.0f;
	if (horizontal == 0) 
		wallX = player.GetPosition().y + distance * rayDir.y;
	else           
		wallX = player.GetPosition().x + distance * rayDir.x;
	wallX -= floor((wallX));
	return Ray(distance, horizontal, map[(int)roundedCoords.x][(int)roundedCoords.y], wallX);
}
