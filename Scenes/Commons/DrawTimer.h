#pragma once

class DrawTimer
{
private:

	int m_flame, m_seconds;

	SimpleMath::Vector2 m_pos;
	SimpleMath::Vector2 m_rage;
	SimpleMath::Color m_color;

public:
	DrawTimer(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage);
	~DrawTimer();

	void Update(float timer);

	void Reset();

	void SetColor(SimpleMath::Color color)	{ m_color = color; }
	SimpleMath::Color GetColor()			{ return m_color; }
	SimpleMath::Vector3 GetColorRGB()		{ return SimpleMath::Vector3(m_color.R(), m_color.G(), m_color.B());}

	void SetPos(SimpleMath::Vector2 position) { m_pos = position; }

	// State EndÇÃä‘Ç≈åƒÇ—èoÇ∑Ç±Ç∆
	void TimerDraw();

	int GetFlame() { return m_flame; }
	int GetSeconds() { return m_seconds; }

};