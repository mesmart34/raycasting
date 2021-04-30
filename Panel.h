#pragma once
#include "UIElement.h"
#include <vector>
#include "Base.h"

class Panel : public UIElement
{
public:
	Panel() = default;
	Panel(const vec2& position, const vec2& size, const uint32_t color);

	void Update(const float deltaTime) override;

private:
	

};