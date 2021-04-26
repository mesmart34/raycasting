#pragma once
#include "UIElement.h"
#include "Input.h"

class Label : public UIElement
{
public:
	Label() = default;
	Label(const std::string& text, const vec2& position, const vec2& size, const uint32_t color);

	std::string GetText() const;
	void SetText(const std::string& text);


	void Update() override;

private:
	std::string m_text;

};