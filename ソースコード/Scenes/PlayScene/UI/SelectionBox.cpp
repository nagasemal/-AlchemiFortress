#include "pch.h"
#include "SelectionBox.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/GameData/UserInterfase.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/SoundData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/GameData/Easing.h"

#include "Scenes/Commons/DrawKey.h"


#define TEXT_SIZE_W 48
// UIの縦横サイズ
#define UITEX_SIZE 64

SelectionBox::SelectionBox(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage)
{

	m_saveData.pos = m_data.pos = pos;
	m_saveData.rage = m_data.rage = rage;

	m_hitMouseFlag = false;
	m_selectFlag = false;

	m_boxColor = Colors::White;

	m_rect = { 0, 0, 64, 64 };

	m_popUITextTimer = 0.0f;

	m_drawKey = std::make_unique<DrawKey>(this);

}

SelectionBox::~SelectionBox()
{
}

void SelectionBox::Update()
{
	m_keySelectFlag = false;

	if (m_selectFlag) SoundData::GetInstance().PlaySystemSE(XACT_WAVEBANK_SYSTEMSE_BUTTONSELECT);

	m_drawKey->Update();
}

void SelectionBox::Draw()
{

	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pSL = &SpriteLoder::GetInstance();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	SimpleMath::Color color		= m_boxColor;

	if (m_hitMouseFlag) color	= m_boxColor * Colors::LightGray;

	if (HoldMouse()) color		= m_boxColor * Colors::Gray;

	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	// 選択BOX
	pSB->Draw(pSL->GetSelectBoxTexture().Get(), box_Pos, &m_rect, color, 0.0f, DirectX::XMFLOAT2(static_cast<float>(m_rect.right) / 2, static_cast<float>(m_rect.bottom) / 2), m_data.rage);

	pSB->End();

	m_drawKey->Draw();

}

void SelectionBox::DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture,
						  RECT rect,SimpleMath::Color pulsTextureCol, float pulsRage, int pulsWidth, int pulsHeight)
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pSL = &SpriteLoder::GetInstance();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	SimpleMath::Color color		= m_boxColor;

	if (m_hitMouseFlag) color	= m_boxColor * Colors::LightGray;

	if (HoldMouse()) color		= m_boxColor * Colors::Gray;

	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	// 選択BOX
	pSB->Draw(pSL->GetSelectBoxTexture().Get(), box_Pos, &m_rect, color, 0.0f, DirectX::XMFLOAT2(static_cast<float>(m_rect.right) / 2, static_cast<float>(m_rect.bottom) / 2), m_data.rage);

	// 中に表示するテクスチャがある場合
	if (pulsTexture)
	{
		pSB->Draw(pulsTexture.Get(), box_Pos, &rect, pulsTextureCol, 0.0f, DirectX::XMFLOAT2(pulsWidth / 2.0f, pulsHeight / 2.0f), pulsRage);
	}

	pSB->End();

	m_drawKey->Draw();

}

void SelectionBox::DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, float rotate)
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// ログの色
	SimpleMath::Color	color = m_boxColor;

	if (m_hitMouseFlag) color = m_boxColor * Colors::LightGray;

	if (HoldMouse()) color = m_boxColor * Colors::Gray;
	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	// 選択BOX
	pSB->Draw(texture.Get(), box_Pos, &m_rect, color, rotate, DirectX::XMFLOAT2(static_cast<float>(m_rect.right) / 2.0f, static_cast<float>(m_rect.bottom) / 2.0f), m_data.rage);

	pSB->End();

	m_drawKey->Draw();

}

void SelectionBox::DrawUI(int UInumber)
{

	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();
	auto pSL = &SpriteLoder::GetInstance();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

		// ログの色
	SimpleMath::Color color = m_boxColor;

	if (m_hitMouseFlag)
	{
		color = m_boxColor * Colors::LightGray;

		m_popUITextTimer += DeltaTime::GetInstance().GetNomalDeltaTime();

	}
	else
	{
		m_popUITextTimer -= DeltaTime::GetInstance().GetNomalDeltaTime();
	}

	if (HoldMouse()) color = m_boxColor * Colors::Gray;

	m_popUITextTimer = std::min(std::max(0.0f, m_popUITextTimer),1.0f);

	// 触れた際に上部に出現する説明用テキストの描画
	SpriteLoder::TextureData texData = pSL->GetUIText();
	RECT textRect = SpriteCutter(TEXT_SIZE_W, texData.height, UInumber, 0);
	pSB->Draw(texData.tex.Get(),
		m_data.pos - SimpleMath::Vector2(0.0f, Easing::EaseInOutQuint(0.0f, TEXT_SIZE_W / 4 + TEXT_SIZE_W * m_data.rage.y, m_popUITextTimer)),
		&textRect,
		SimpleMath::Color(0.0f, 0.0f, 0.0f,m_popUITextTimer),
		1.0f - Easing::EaseOutCirc(0.0f, 1.0f, m_popUITextTimer),
		DirectX::XMFLOAT2(static_cast<float>(TEXT_SIZE_W / 2),static_cast<float>(texData.height / 2)),
		1.0f);

	// 選択BOX描画
	pSB->Draw(pSL->GetSelectBoxTexture().Get(), m_data.pos, &m_rect, color, 0.0f,
		DirectX::XMFLOAT2(static_cast<float>(m_rect.right) / 2,
						  static_cast<float>(m_rect.bottom) / 2), m_data.rage);


	// 画像のサイズ
	RECT srcRect = SpriteCutter(UITEX_SIZE, UITEX_SIZE, UInumber, 0);
	// 中のUI表示
	pSB->Draw(pSL->GetUIIcons().Get(), m_data.pos, &srcRect, SimpleMath::Color(0.0f,0.0f,0.0f, m_boxColor.A()), 0.0f, DirectX::XMFLOAT2(UITEX_SIZE / 2, UITEX_SIZE / 2), 0.8f);


	pSB->End();

	m_drawKey->Draw();


}

void SelectionBox::DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
						  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture,
						  RECT rect, float pulsRage,
						  SimpleMath::Color pulsTextureCol,
						  SimpleMath::Color nomalColor,
						  SimpleMath::Color onColor,
						  SimpleMath::Color pressdColor)
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT srcRect = { 0, 0, 64, 64 };

	// ログの色
	SimpleMath::Color colour = nomalColor;

	if (m_hitMouseFlag) colour = onColor;

	if (HoldMouse()) colour = pressdColor;

	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	// 選択BOX
	pSB->Draw(texture.Get(), box_Pos, &srcRect, colour, 0.0f, DirectX::XMFLOAT2(64 / 2, 64 / 2), m_data.rage);

	// 中に表示するテクスチャがある場合
	if (pulsTexture)
	{
		pSB->Draw(pulsTexture.Get(),box_Pos, &rect, pulsTextureCol, 0.0f, DirectX::XMFLOAT2(static_cast<float>(rect.right / 2), static_cast<float>(rect.bottom / 2)), pulsRage);
	}

	pSB->End();

	m_drawKey->Draw();
}

void SelectionBox::DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, RECT rect, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture, RECT pulsTexRect, SimpleMath::Color pulsTextureCol, SimpleMath::Color nomalColor, SimpleMath::Color onColor, SimpleMath::Color pressdColor)
{

	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	// ログの色
	SimpleMath::Color colour	= nomalColor;

	if (m_hitMouseFlag) colour	= onColor;

	if (HoldMouse()) colour		= pressdColor;

	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	// 選択BOX
	pSB->Draw(texture.Get(), box_Pos, &rect, colour, 0.0f, DirectX::XMFLOAT2((float)rect.right / 2.0f, (float)rect.bottom / 2.0f), m_data.rage);

	// 中に表示するテクスチャがある場合
	if (pulsTexture)
	{
		pSB->Draw(pulsTexture.Get(), box_Pos, &pulsTexRect, pulsTextureCol, 0.0f, DirectX::XMFLOAT2(64 / 2, 64 / 2), 0.8f);
	}

	pSB->End();


	m_drawKey->Draw();

}

void SelectionBox::DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, float rotate, int width, int height, int beside, int vertical)
{

	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// ログの色
	SimpleMath::Color	color = m_boxColor;

	if (m_hitMouseFlag) color = m_boxColor * Colors::LightGray;

	if (HoldMouse()) color = m_boxColor * Colors::Gray;
	SimpleMath::Vector2 box_Pos = { m_data.pos.x,m_data.pos.y };

	RECT rect = SpriteCutter(width, height, beside, vertical);

	// 選択BOX
	pSB->Draw(texture.Get(), box_Pos, &rect, color, rotate, DirectX::XMFLOAT2(static_cast<float>(m_rect.right) / 2.0f, static_cast<float>(m_rect.bottom) / 2.0f), m_data.rage);

	pSB->End();

	m_drawKey->Draw();

}

