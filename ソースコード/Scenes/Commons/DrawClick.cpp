#include "pch.h"
#include "DrawClick.h"

#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"

#include "NecromaLib/GameData/SpriteCutter.h"

DrawClick::DrawClick(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage)
{
	m_data.pos	= pos;
	m_data.rage = rage;

	m_animationData = 0.0f;

}

DrawClick::~DrawClick()
{
}

void DrawClick::Initialize()
{
}

void DrawClick::Update()
{

	m_animationData += DeltaTime::GetInstance().GetNomalDeltaTime();

	if (m_animationData.MaxCheck()) m_animationData = 0.0f;

}

void DrawClick::Draw()
{

	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	//pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 内部BOX (マシンUIを配置する箇所)
	SpriteLoder::TextureData texData = pSL.GetClickMousePointer();

	// 画像のサイズ
	RECT srcRect = { 0, 0, texData.width, texData.height };

	SimpleMath::Vector2 pos = { m_data.pos.x - m_animationData, m_data.pos.y - m_animationData };

	pSB->Draw(texData.tex.Get(), pos, &srcRect, Colors::White, 0.0f,
		DirectX::XMFLOAT2(static_cast<float>(texData.width / 2),static_cast<float>(texData.height / 2)), 0.5f);

	//pSB->End();

}

void DrawClick::Finalize()
{
}
