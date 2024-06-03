#include "pch.h"
#include "RenderOption.h"

#include "Scenes/Commons/DrawSlider.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/GameData/SpriteCutter.h"

#include "NecromaLib/Singleton/SoundData.h"


#define RAGE 	SimpleMath::Vector2(0.6f, 0.6f)

RenderOption::RenderOption()
{
	m_userBar_BGM			= std::make_unique<DrawSlider>(SimpleMath::Vector2(400, 200), RAGE);
	m_userBar_SE			= std::make_unique<DrawSlider>(SimpleMath::Vector2(400, 280), RAGE);
	m_userBar_SystemSE		= std::make_unique<DrawSlider>(SimpleMath::Vector2(400, 360), RAGE);

	m_cancelButton = std::make_unique<SelectionBox>(SimpleMath::Vector2(1280 / 2, 120),SimpleMath::Vector2(1.0f,1.0f));
}

RenderOption::~RenderOption()
{
}

void RenderOption::Initialize()
{
}

void RenderOption::Update()
{
	SoundData& pSound = SoundData::GetInstance();

	m_userBar_BGM		->Update();
	pSound.SetBGMVol(m_userBar_BGM->GetValue(), m_userBar_BGM->GetValue());

	m_userBar_SE		->Update();
	pSound.SetSEVol(m_userBar_SE->GetValue(), m_userBar_SE->GetValue());

	m_userBar_SystemSE	->Update();
	pSound.SetSystemSEVol(m_userBar_SystemSE->GetValue(), m_userBar_SystemSE->GetValue());

	m_cancelButton->HitMouse();

}

void RenderOption::Render()
{

	// ”wŒi•‚­‚·‚é
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();
	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = (float)device->GetOutputSize().right;
	float height = (float)device->GetOutputSize().bottom;

	RECT rect = { 0,0,800,600 };

	pSB->Draw(pSL.GetMagicRule(0).Get(), SimpleMath::Vector2(width / 2.0f, height / 2.0f),
		&rect,
		SimpleMath::Color(0.0f, 0.0f, 0.0f, 0.5f),
		0.0f,
		DirectX::XMFLOAT2{ 800.0f / 2.0f,600.0f / 2.0f }, 2.0f);

	pSD.GetSpriteBatch()->End();

	m_userBar_BGM		->Render(SpriteLoder::UIICON_TYPE::SOUND);
	m_userBar_SE		->Render(SpriteLoder::UIICON_TYPE::SE);
	m_userBar_SystemSE	->Render(SpriteLoder::UIICON_TYPE::SYSTEMSE);

	m_cancelButton->DrawUI(SpriteLoder::UIICON_TYPE::CANCEL);
}

void RenderOption::Finalize()
{
	m_userBar_BGM		->Finalize();
	m_userBar_SE		->Finalize();
	m_userBar_SystemSE	->Finalize();
}

bool RenderOption::GetHitCancelButton()
{
	return m_cancelButton->ClickMouse();
}
