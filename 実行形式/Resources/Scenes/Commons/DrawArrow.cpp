#include "pch.h"
#include "DrawArrow.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/Commons/DrawKey.h"

#define TEXTURE_SIZE 64

DrawArrow::DrawArrow(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage,int direction)
{
	m_saveData.pos = m_data.pos = pos;
	m_saveData.rage = m_data.rage = rage;
	m_direction = direction;

	m_boxColor = SimpleMath::Color(Colors::White);

	m_drawKey = std::make_unique<DrawKey>(this);

}

DrawArrow::~DrawArrow()
{
}

void DrawArrow::Update()
{
	m_keySelectFlag = false;


	m_drawKey->Update();

}

void DrawArrow::Draw()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pSL = &SpriteLoder::GetInstance();

	// 画像のサイズ
	RECT srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };
	SimpleMath::Color colour = m_boxColor;

	if (m_hitMouseFlag) colour = m_boxColor * Colors::LightGray;

	if (HoldMouse()) colour = m_boxColor * Colors::Gray;


	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 選択BOX
	pSB->Draw(pSL->GetUIArrow().Get(),
		m_data.pos,
		&srcRect,
		colour,
		XMConvertToRadians(90.0f * m_direction),
		DirectX::XMFLOAT2(TEXTURE_SIZE / 2, TEXTURE_SIZE / 2),
		m_data.rage);

	pSB->End();

	m_drawKey->Draw();

}


