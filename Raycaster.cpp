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

	auto ray = Ray();
	auto hit = false;
	do
	{
		auto last = map;
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


		if (map[(int)roundedCoords.x][(int)roundedCoords.y] == 99)
		{
			auto dir = horizontal;
			//need to backup by the end
			auto last_x = roundedCoords.x;
			auto last_y = roundedCoords.y;

			auto distance = 0.0f;
			auto hit_point = 0.0f;

			auto state_hit_point = 0.0f;
			if (horizontal == 0)
				state_hit_point = player.GetPosition().y + ((roundedCoords.x + step.x * 0.5f - player.GetPosition().x + (1 - step.x) / 2) / rayDir.x) * rayDir.y;
			else
				state_hit_point = player.GetPosition().x + ((roundedCoords.y + step.y * 0.5f - player.GetPosition().y + (1 - step.y) / 2) / rayDir.y) * rayDir.x;
			state_hit_point -= floor((state_hit_point));
			//do an additional iteration to get next hit point, exactly like in cast_ray function but here we need the distance too
			if (sideDist.x < sideDist.y)
			{
				roundedCoords.x += step.x;
				horizontal = 0;
				distance = (roundedCoords.x - player.GetPosition().x + (1 - step.x) / 2) / rayDir.x;
			}
			else
			{
				roundedCoords.y += step.y;
				horizontal = 1;
				distance = (roundedCoords.y - player.GetPosition().y + (1 - step.x) / 2) / rayDir.y;
			}

			//getting a hit point from an additional iteration 
			if (horizontal == 0)
				hit_point = player.GetPosition().y + distance * rayDir.y;
			else
				hit_point = player.GetPosition().x + distance * rayDir.x;
			hit_point -= floor((hit_point));

			// getting the offset relative to the direction a ray is casted with
			auto offset = (dir == 0 ? rayDir.x : rayDir.y) > 0 ? 0.5 : 0.0;

			auto opening_offset = 0.3f;
			auto texture_offset = opening_offset;

			//check if ray actually hit a thin wall
			if (hit_point >= 0.0 + offset && hit_point < 0.5 + offset || (dir == 0 ? (last_x != roundedCoords.x) : (last_y != roundedCoords.y)))
			{
				//printf("%f\n", player_frac);
				if (state_hit_point < opening_offset)
				{
					roundedCoords.x = last_x;
					roundedCoords.y = last_y;
					
				}
				else {
					horizontal = dir;
					ray.Door = true;
					hit = true;
					roundedCoords.x = last_x + (dir == 0 ? step.x * 0.5f : 0);
					roundedCoords.y = last_y + (dir == 1 ? step.y * 0.5f : 0);
				}


			}
		}else if (map[(int)roundedCoords.x][(int)roundedCoords.y] > 0)
			hit = true;
	} while (!hit);
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
	ray.Distance = distance;
	ray.Horizontal = horizontal;
	ray.WallX = wallX;
	ray.Id = map[(int)roundedCoords.x][(int)roundedCoords.y];
	return ray;
}


