#pragma once

#include <string>
#include "vec2.h"
#include <vector>
#include "Base.h"

class UIElement
{
public:
	UIElement(const vec2& position, const vec2& size, const uint32_t color);
	virtual ~UIElement() = default;

	virtual void Update(const float deltaTime) {};

	vec2 GetPosition() const;
	void SetLocalPosition(const vec2& position);
	void SetSize(const vec2& size);
	vec2 GetLocalPosition() const;
	vec2 GetSize() const;
	uint32_t GetColor() const;
	uint32_t GetBackgroundColor() const;
	void SetBackgroundColor(const uint32_t color);
	void AddChildren(const Ref<UIElement>& m_child);
	std::vector<Ref<UIElement>>& GetChildren();
	bool IsEnabled() const;
	void SetEnable(const bool value);
	void SetParent(const Ref<UIElement> parent);
	Ref<UIElement> GetParent() const;

	virtual void OnMouseEnter() {};
	virtual void OnMouseLeave() {};
	virtual void OnMouseStay() {};

protected:
	vec2 m_position;
	vec2 m_size;
	uint32_t m_color;
	uint32_t m_backgroundColor;
	std::vector<Ref<UIElement>> m_children;
	Ref<UIElement> m_parent;
	bool m_enabled;
private:
	bool m_mouseHover;

};
