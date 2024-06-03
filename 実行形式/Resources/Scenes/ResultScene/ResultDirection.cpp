#include "pch.h"
#include "ResultDirection.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#define ALPHA_VAL 0.2f			// 右上魔法陣の透明度
#define ROTATE_VAL 0.5f			// 右上魔法陣の回転角度
#define SCALE_VAL 1.0f			// 右上魔法陣の大きさ

ResultDirection::ResultDirection():
	m_animationData(),
	m_rotateNowFlag()
{
}

ResultDirection::~ResultDirection()
{
}

void ResultDirection::Update()
{
	InputSupport* pINP = &InputSupport::GetInstance();

	//　====================[　左クリックをした際の処理　]
	//　　|=>　変数のリセット
	if (pINP->LeftButton_Press() && !m_rotateNowFlag)
	{
		m_animationData = 0.0f;
		m_rotateNowFlag = true;
	}

	//　====================[　魔法陣を動かす　]
	if (m_rotateNowFlag)
	{
		m_animationData += DeltaTime::GetInstance().GetDeltaTime();
		m_animationData.anim = Easing::EaseInOutCubic(0.0f, XMConvertToRadians(90.0f), m_animationData);

		// 右上の魔法陣の回転が終わったらシーン遷移を開始するため
		// アニメーション変数が最大値に至っていなければTrueとする
		m_rotateNowFlag = !m_animationData.MaxCheck();

	}



}

void ResultDirection::Render()
{
	// 必要なリソースの確保
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder::TextureData texData = SpriteLoder::GetInstance().GetMachineMagicCircleTexture(1);
	auto pSB = pSD.GetSpriteBatch();
	auto device = pSD.GetDeviceResources();

	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	//　====================[　UI描画の開始　]
	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	//　　|=> 画像のサイズ
	RECT rect_circle = { 0, 0, texData.width, texData.height };

	//　　|=> 画像の位置設定
	SimpleMath::Vector2 box_Pos = { (float)width / 2.0f,(float)height / 2.0f };

	//　　|=>  右上にクリック時回転する魔法陣を描画
	pSB->Draw(texData.tex.Get(),
		box_Pos,
		&rect_circle,
		SimpleMath::Color(1.0f, 1.0f, 1.0f, ALPHA_VAL),
		m_animationData.anim,
		DirectX::XMFLOAT2(static_cast<float>(texData.width / 2), static_cast<float>(texData.height / 2)),
		SCALE_VAL);

	pSB->End();
}
