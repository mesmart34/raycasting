#include "Raycaster.h"

using namespace std;

Ray Raycaster::CastRay(const int strip, const int width, const Player& player, const Map& map) const
{
	auto cast = Cast();
	cast.Rounded = vec2::floor(player.GetPosition());
	cast.CameraX = 2 * strip / (float)width - 1;
	cast.Direction = vec2(
		player.GetDirection().x + player.GetPlane().x * cast.CameraX,
		player.GetDirection().y + player.GetPlane().y * cast.CameraX
	);
	cast.DeltaDistance = vec2(
		(cast.Direction.y == 0) ? 0 : ((cast.Direction.x == 0) ? 1 : fabs(1 / cast.Direction.x)),
		(cast.Direction.x == 0) ? 0 : ((cast.Direction.y == 0) ? 1 : fabs(1 / cast.Direction.y))
	);
	if (cast.Direction.x < 0)
		cast.SideDistance.x = (player.GetPosition().x - cast.Rounded.x) * cast.DeltaDistance.x;
	else
		cast.SideDistance.x = (cast.Rounded.x + 1.0f - player.GetPosition().x) * cast.DeltaDistance.x;
	cast.Step.x = cast.Direction.x < 0 ? -1 : 1;

	if (cast.Direction.y < 0)
		cast.SideDistance.y = (player.GetPosition().y - cast.Rounded.y) * cast.DeltaDistance.y;
	else
		cast.SideDistance.y = (cast.Rounded.y + 1.0f - player.GetPosition().y) * cast.DeltaDistance.y;
	cast.Step.y = cast.Direction.y < 0 ? -1 : 1;
	cast.Id = map.GetIndexAt((int)cast.Rounded.x, (int)cast.Rounded.y);
	auto ray = Ray();

	auto steps = 0;
	do
	{
		steps++;
		if (steps > 20)
			break;
		MakeStep(cast);
		if (cast.DoorBox)
			cast.Hit = true;
		else {
			cast.Id = map.GetIndexAt((int)cast.Rounded.x, (int)cast.Rounded.y);
			if (cast.Id == 99 || cast.Id == 98)
			{
				if (cast.Vertical)
					ProcessVerticalDoor(cast, player, map);
				else 
					ProcessHorizontalDoor(cast, player, map);

			}
			else if (cast.Id > 0)
			{
				auto coords = vec2::floor(player.GetPosition());
				/*if (map.GetIndexAt(coords.x, coords.y) == 98)
				{
					if (map.GetIndexAt((int)coords.x, (int)coords.y - 1) == 98 || map.GetIndexAt((int)coords.x, (int)coords.y + 1) == 98 && !cast.Vertical)
						cast.DoorBox = true;
					if ((map.GetIndexAt((int)coords.x - 1, (int)coords.y) == 98 || map.GetIndexAt((int)coords.x + 1, (int)coords.y) == 98) && cast.Vertical)
						cast.DoorBox = true;
				}*/
				
				cast.Hit = true;
			}
		}
	} while (!cast.Hit);
	cast.Distance = CalculateDistance(cast, player);
	cast.WallX = CalculateWallX(cast, player);
	cast.WallX -= floor(cast.WallX);
	ray.Distance = cast.Distance;
	ray.Horizontal = !cast.Vertical;
	ray.WallX = cast.WallX;
	ray.Id = cast.Id;
	ray.Door = cast.Door;
	ray.DoorBox = cast.DoorBox;
	ray.TextureOffset = cast.TextureOffset;
	return ray;
}

float Raycaster::CalculateDistance(const Cast& cast, const Player& player) const
{
	if (cast.Vertical)
		return (cast.Rounded.y - player.GetPosition().y + (1 - cast.Step.y) / 2) / cast.Direction.y;
	else
		return (cast.Rounded.x - player.GetPosition().x + (1 - cast.Step.x) / 2) / cast.Direction.x;
}

float Raycaster::CalculateWallX(const Cast& cast, const Player& player) const
{
	if (!cast.Vertical)
		return player.GetPosition().y + cast.Distance * cast.Direction.y;
	else
		return player.GetPosition().x + cast.Distance * cast.Direction.x;
}

void Raycaster::ProcessVerticalDoor(Cast& cast, const Player& player, const Map& map) const
{
	auto temp = cast;
	MakeStep(temp);
	temp.Distance = CalculateDistance(temp, player);
	temp.WallX = CalculateWallX(temp, player);
	temp.WallX -= floor(temp.WallX);
	auto playerDirection = cast.Direction.y < 0 ? 1 : -1;
	auto doorOffset = map.GetDoorInfo(cast.Rounded.x, cast.Rounded.y);
	//auto doorOffset = 1.0f;
	auto isDoor = playerDirection > 0 ? temp.WallX < 0.5f : temp.WallX >= 0.5f;
	auto temp2 = cast;
	auto distance = CalculateDistance(temp2, player);
	temp2.Distance = distance;
	auto wallX = CalculateWallX(temp2, player);
	wallX -= floorf(wallX);
	cast.TextureOffset = doorOffset;
	if ((isDoor || temp.Vertical == cast.Vertical))
	{
		temp2 = cast;
		temp2.Rounded.y += temp2.Step.y * 0.5f;
		temp2.Distance = CalculateDistance(temp2, player);
		temp2.WallX = CalculateWallX(temp2, player);
		temp2.WallX -= floor(temp2.WallX);
		if (temp2.WallX <= doorOffset && temp2.WallX > 0)
		{
			if (cast.Rounded.y == temp.Rounded.y)
				cast.DoorBox = true;
			return;
		}
		cast.Rounded.y += cast.Step.y * 0.5f;
		cast.Door = true;
	}
	else
	{
		cast = temp;
		cast.DoorBox = true;
	}
	cast.Hit = true;
}

void Raycaster::ProcessHorizontalDoor(Cast& cast, const Player& player, const Map& map) const
{
	auto temp = cast;
	MakeStep(temp);
	temp.Distance = CalculateDistance(temp, player);
	temp.WallX = CalculateWallX(temp, player);
	temp.WallX -= floor(temp.WallX);
	auto playerDirection = cast.Direction.x < 0 ? 1 : -1;
	auto doorOffset = map.GetDoorInfo(cast.Rounded.x, cast.Rounded.y);
	auto isDoor = playerDirection > 0 ? temp.WallX < 0.5f : temp.WallX >= 0.5f;
	auto temp2 = cast;
	auto distance = CalculateDistance(temp2, player);
	temp2.Distance = distance;
	auto wallX = CalculateWallX(temp2, player);
	wallX -= floorf(wallX);
	cast.TextureOffset = doorOffset;
	if ((isDoor || temp.Vertical == cast.Vertical))
	{
		temp2 = cast;
		temp2.Rounded.x += temp2.Step.x * 0.5f;
		temp2.Distance = CalculateDistance(temp2, player);
		temp2.WallX = CalculateWallX(temp2, player);
		temp2.WallX -= floor(temp2.WallX);
		if (temp2.WallX <= doorOffset && temp2.WallX > 0)
		{

			if (cast.Rounded.x == temp.Rounded.x)
				cast.DoorBox = true;
			return;
		}
		cast.Rounded.x += cast.Step.x * 0.5f;
		cast.Door = true;
	}
	else
	{
		cast = temp;
		cast.DoorBox = true;
	}
	cast.Hit = true;
}

void Raycaster::MakeStep(Cast& cast) const
{
	if (cast.SideDistance.x < cast.SideDistance.y)
	{
		cast.SideDistance.x += cast.DeltaDistance.x;
		cast.Rounded.x += cast.Step.x;
		cast.Vertical = false;
	}
	else
	{
		cast.SideDistance.y += cast.DeltaDistance.y;
		cast.Rounded.y += cast.Step.y;
		cast.Vertical = true;
	}
}


