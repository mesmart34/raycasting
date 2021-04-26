#pragma once

#include "MathUtils.h"

class Door
{
public:
	Door() = default;

	void Open();
	void Update(const float deltaTime);
	bool IsOpened() const;
	float GetTextureOffset() const;

private:
	enum State
	{
		Opened, Closed, Opening
	};
	float m_offset;
	float m_timer;
	State m_state = Closed;
	bool m_pressed;
};

