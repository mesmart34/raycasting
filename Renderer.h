#pragma once
#include "SDL/SDL.h"
#include "MathUtils.h"
#include "Window.h"
#include "Raycaster.h"
#include "Texture.h"
#include <iostream>
#include <vector>
#include "Sprite.h"
#include "Object.h"
#include <algorithm>
#include "Font.h"
#include <thread>

class Renderer
{
public:
	Renderer() = default;
	Renderer(SDL_Window* window, const int width, const int height);

	void Clear();
	inline void PutPixel(const int x, const int y, const Uint32 color);
	void DrawStrip(const Ray& ray, const Texture& atlas, const int x);
	void Draw();
	void DrawRect(Renderer& renderer, const vec2& start, const vec2& size, const uint32_t color, const int width);
	void DrawColumn(const std::vector<uint32_t>& data, const int x);
	void DrawSprite(const Sprite& sprite, const vec2& position, const Player& player);
	void SortObjects(std::vector<Object*>& m_objects, const Player& m_player);
	int GetWidth() const;
	int GetHeight() const;
	void DrawText(const std::string& text, const Font& font, const vec2& position, const uint32_t color);
	void DrawTexture(const std::shared_ptr<Texture>& texture, const uint32_t drawColor, const vec2& position, const int start, const int letterWidth, const int letterHeight);
	
private:
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;
	uint32_t* m_data;
	int m_width, m_height;
	std::vector<float> m_zBuffer;
	float m_spriteScale;
	uint32_t m_transparentColor;
};

