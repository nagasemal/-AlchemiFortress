#include "pch.h"
#include "DrawTimer.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "NecromaLib/GameData/SpriteCutter.h"

#define TEXTURE_RAGE 64

DrawTimer::DrawTimer(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage)
{
	m_flame		= 0;
	m_seconds	= 0;

	m_pos = pos;
	m_rage = rage;
	m_color = (SimpleMath::Color)Colors::White;

}

DrawTimer::~DrawTimer()
{
}

void DrawTimer::Update(float timer)
{

	m_seconds = (int)timer;

}

void DrawTimer::Reset()
{
	m_flame = 0;
	m_seconds = 0;
}

void DrawTimer::TimerDraw()
{

	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	int seconds, seconds2, minuts, minuts2;

	seconds = m_seconds % 60 % 10;
	seconds2 = m_seconds % 60 / 10;
	minuts = m_seconds / 60 % 10;
	minuts2 = m_seconds / 60 / 10;

	RECT TenFirst[4] = { RECT(),RECT(),RECT(),RECT() };
	TenFirst[0].left = TEXTURE_RAGE * seconds;
	TenFirst[0].top = 0;
	TenFirst[0].right = TEXTURE_RAGE + TEXTURE_RAGE * seconds;
	TenFirst[0].bottom = TEXTURE_RAGE;

	TenFirst[1].left = TEXTURE_RAGE * seconds2;
	TenFirst[1].top = 0;
	TenFirst[1].right = TEXTURE_RAGE + TEXTURE_RAGE * seconds2;
	TenFirst[1].bottom = TEXTURE_RAGE;

	TenFirst[2].left = TEXTURE_RAGE * minuts;
	TenFirst[2].top = 0;
	TenFirst[2].right = TEXTURE_RAGE + TEXTURE_RAGE * minuts;
	TenFirst[2].bottom = TEXTURE_RAGE;

	TenFirst[3].left = TEXTURE_RAGE * minuts2;
	TenFirst[3].top = 0;
	TenFirst[3].right = TEXTURE_RAGE + TEXTURE_RAGE * minuts2;
	TenFirst[3].bottom = TEXTURE_RAGE;

	SimpleMath::Vector2 origin(TEXTURE_RAGE / 2, TEXTURE_RAGE / 2);

	for (int i = 0; i < 4; i++)
	{
		int pulsX = 0;
		if (i < 2) pulsX = TEXTURE_RAGE / 2;

		SimpleMath::Vector2 pos(m_pos.x - m_rage.x * (i * (TEXTURE_RAGE / 2)) + pulsX, m_pos.y);
		pSB->Draw(pSL.GetNumberTexture().Get(), pos, &TenFirst[i] , m_color, 0.0f , origin , m_rage);
	}

	RECT rect = SpriteCutter(TEXTURE_RAGE, TEXTURE_RAGE,1,0);

	// [:]•`‰æ
	pSB->Draw(pSL.GetSymbolTexture().Get(), SimpleMath::Vector2(m_pos.x - (m_rage.x * (TEXTURE_RAGE / 2) / 2), m_pos.y), &rect, m_color, 0.0f, origin, m_rage);

}
