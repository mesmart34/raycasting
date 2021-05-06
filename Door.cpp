#include "pch.h"
#include "Door.h"

void Door::Open()
{
	m_state = State::Opened;
	m_timer = 0.0f;
}

void Door::Update(const float deltaTime)
{

	if (m_state == State::Opened)
	{
		m_offset += deltaTime;
		if (m_timer <= 10)
			m_timer += deltaTime;
		else
			m_state = State::Closed;
	}
	if (m_state == State::Closed)
		m_offset -= deltaTime;
	m_offset = MathUtils::Clamp(m_offset, 0, 1);
}

bool Door::IsOpened() const
{
    return m_state == State::Opened && m_offset == 1.0f;
}

float Door::GetTextureOffset() const
{
    return m_offset;
}
