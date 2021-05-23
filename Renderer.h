#pragma once

#include "Base.h"
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
#include "UIElement.h"
#include "Label.h"
#include "Button.h"
#include "Panel.h"
#include "TextBox.h"
#include "Player.h"


class Renderer
{
public:
	Renderer() = default;
	Renderer(const int width, const int height);
	~Renderer();

	void Init(const int width, const int height);
	void Resize(const int width, const int height);
	void Clear();
	inline void PutPixel(const int x, const int y, const Uint32 color);
	inline uint32_t GetPixel(const int x, const int y);
	void DrawStrip(const Ray& ray, const Ref<Texture>& atlas, const int x, const bool isPlayerInside);
	void Draw();
	void DrawRect(const vec2& start, const vec2& size, const uint32_t color);
	void DrawColumn(const std::vector<uint32_t>& data, const int x);
	void DrawSprite(const Sprite& sprite, const vec2& position, const Scope<Player>& player);
	void SortObjects(std::vector<Ref<Object>>& m_objects, const vec2& cameraPosition);
	int GetWidth() const;
	int GetHeight() const;
	void DrawText(const std::string& text, const Font& font, const vec2& position, const float size, const uint32_t color);
	void DrawTexture(const Ref<Texture>& texture, const uint32_t drawColor, const vec2& position, const int start, const int letterWidth, const int letterHeight, const vec2& size);
	float GetScale() const;
	void SetVerticalOffset(const float value);
	inline uint32_t GetBlendedColor(const uint32_t a, const uint32_t b) const;
	void DrawUIElement(Ref<UIElement> element);
	void SetMaxDistance(const float maxDistance);
private:
	void DrawUIElementWithChildren(Ref<UIElement> element);
	void DrawButton(const Button* const button);
	void DrawLabel(const Label* const label);
	void DrawTextBox(const TextBox* const textBox);

	uint32_t* m_data;
	GLuint m_quadTextureId;
	int m_width, m_height;
	std::vector<float> m_zBuffer;
	float m_scale;
	uint32_t m_transparentColor;
	float m_renderVerticalOffset;
	Font m_miniFont;
	Font m_font;
	float m_maxDistance;
	float m_fogDistance;
	uint32_t m_fogColor;
};

