#include "pch.h"
#include "Panel.h"

Panel::Panel(const vec2& position, const vec2& size, const uint32_t color)
	: UIElement(position, size, color)
{

}


void Panel::Update(const float deltaTime)
{
	for (auto& el : m_children)
	{
		el->Update(deltaTime);
	}
}
