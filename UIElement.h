#pragma once

#include <string>
#include "vec2.h"

class UIElement
{
public:
	UIElement(const vec2& position, const vec2& size, const uint32_t color);
	virtual ~UIElement() = default;

	virtual void Update() = 0;

	vec2 GetPosition() const;
	vec2 GetSize() const;
	uint32_t GetColor() const;
	uint32_t GetBackgroundColor() const;

protected:
	vec2 m_position;
	vec2 m_size;
	uint32_t m_color;
	uint32_t m_backgroundColor;
};
