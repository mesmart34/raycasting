#include "UIElement.h"


vec2 UIElement::GetPosition() const
{
	return m_position;
}

vec2 UIElement::GetSize() const
{
	return m_size;
}

uint32_t UIElement::GetColor() const
{
	return m_color;
}

uint32_t UIElement::GetBackgroundColor() const
{
	return m_backgroundColor;
}



UIElement::UIElement(const vec2& position, const vec2& size, const uint32_t color)
: m_position(position), m_size(size), m_color(color)
{
}
