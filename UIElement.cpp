#include "pch.h"
#include "UIElement.h"

UIElement::UIElement(const vec2& position, const vec2& size, const uint32_t color)
	: m_position(position), m_size(size), m_backgroundColor(color), m_enabled(true), m_focused(false)
{
}

vec2 UIElement::GetPosition() const
{
	return m_position + (m_parent != nullptr ? m_parent->GetPosition() : vec2(0, 0));
}

bool UIElement::IsFocused() const
{
	return m_focused;
}

bool UIElement::IsMouseInside(const int x, const int y) const
{
	auto result = x >= GetPosition().x && x < GetPosition().x + m_size.x && y >= GetPosition().y && y < GetPosition().y + m_size.y;
	return result;
}

void UIElement::SetLocalPosition(const vec2& position)
{
	m_position = position;
}

void UIElement::SetSize(const vec2& size)
{
	m_size = size;
}

vec2 UIElement::GetLocalPosition() const
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

void UIElement::SetBackgroundColor(const uint32_t color)
{
	m_backgroundColor = color;
}

void UIElement::AddChild(const Ref<UIElement>& m_child)
{
	m_child->m_parent = Ref<UIElement>(this);
	m_children.push_back(m_child);
}

std::vector<Ref<UIElement>>& UIElement::GetChildren()
{
	return m_children;
}

bool UIElement::IsEnabled() const
{
	return m_enabled;
}

void UIElement::SetEnable(const bool value)
{
	m_enabled = value;
}

void UIElement::SetParent(const Ref<UIElement> parent)
{
	m_parent = parent;
}

Ref<UIElement> UIElement::GetParent() const
{
	return m_parent;
}
