#pragma once
#include "SDL/SDL.h"
#include <vector>
#include "vec2.h"
#include <cmath>
#include <iostream>

struct Ray
{
	Ray(const float distance, const bool horizontal, const int id)
	{
		Distance = distance;
		Horizonal = horizontal;
		Id = id;
	}
	float Distance;
	bool Horizonal;
	int Id;
};

class Game {
public:
	Game(const int width, const int height);

	void Run();
private:
	void HandleEvents();
	void Update(const float deltaTime);
	void Render();
	void ClearScreen(uint32_t data[]);
	uint32_t PackRGBA(const SDL_Color color) const;
	void DrawWorld(uint32_t data[]);
	float DegToRag(const float deg) const;
	Ray CastRay(const int strip) const;
	void DrawWall(uint32_t data[], const Ray& ray, const int x);
	vec2 RotateVector(const vec2& other, const float angle) const;

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;
	bool m_running;
	int m_width, m_height;
	static std::vector<std::vector<int>> m_map;
	float fieldOfView;
	vec2 playerPosition;
	float playerAngle;
	float wallHeight;
	float viewDistance;
	vec2 playerPlane;
	vec2 direction;
};