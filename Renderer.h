#pragma once
#include "SDL/SDL.h"
#include "MathUtils.h"
#include "Window.h"
#include "Ray.h"
#include "Texture.h"
#include <iostream>

class Renderer
{
public:
	Renderer() = default;
	Renderer(const Window& window, const int width, const int height);

	void Clear();
	void PutPixel(const int x, const int y, const Uint32 color);
	void DrawWallStrip(const Ray& ray, const Texture& texture, const int x);
	void Draw();
	void DrawRect(Renderer& renderer, const vec2& start, const vec2& size, const uint32_t color, const int width);

	int GetWidth() const;
	int GetHeight() const;

private:
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;
	uint32_t* m_data;
	int m_width, m_height;
};

