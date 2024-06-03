#include "pch.h"
#include "Fade.h"

#include <WICTextureLoader.h>


#include "DeviceResources.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/SpriteCutter.h"

Fade* Fade::instance = nullptr;

Fade::Fade():
	mScreenH(),
	mScreenW(),
	mInFlag(),
	mOutFlag(),
	mNowFlag(false),
	mTime(),
	mSpeed(1.5f),
	mFadeTexture()
{
	
}

void Fade::Initialize(int screenWidth, int screenHeight)
{
	mScreenW = screenWidth;
	mScreenH = screenHeight;

	mInFlag = false;
	mOutFlag = false;

	mTime = 0.0f;

	// テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Fade.png",
		nullptr,
		mFadeTexture.ReleaseAndGetAddressOf()
	);

}

void Fade::Render()
{

	auto pSB = ShareData::GetInstance().GetSpriteBatch();

	pSB->Begin
	(DirectX::SpriteSortMode_Deferred, ShareData::GetInstance().GetCommonStates()->NonPremultiplied());

	// フェードの演出記載
	if (mNowFlag)
	{
		// 画面のサイズ
		SimpleMath::Rectangle screenBound = SimpleMath::Rectangle(0, 0, mScreenW,mScreenH);

		// フェードの色
		SimpleMath::Color colour = SimpleMath::Color(0.0f, 0.0f, 0.0f, mTime);

		// 描画
		pSB->Draw(mFadeTexture.Get(), screenBound, colour);
	}

	pSB->End();

}

void Fade::Finalize()
{
	mFadeTexture.Reset();
}

void Fade::Update(DX::StepTimer const& timer)
{

	FadeInUpdate(timer);
	FadeOutUpdate(timer);

}

void Fade::FadeInUpdate(DX::StepTimer const& timer)
{

	if (!mInFlag) return;

	mOutFlag = false;

	mNowFlag = true;

	mTime += float(timer.GetElapsedSeconds()) * mSpeed;

	// 処理


	//終了条件時、InフラグをFalseに
	if (mTime > 1.0f)
	{
		mTime = 1.0f;
		mInFlag = false;
		mOutFlag = true;
	}
}

void Fade::FadeOutUpdate(DX::StepTimer const& timer)
{
	if (!mOutFlag) return;

	mInFlag = false;


	mTime -= float(timer.GetElapsedSeconds()) * mSpeed;

	// 処理

	//終了条件時、OutフラグをFalseに
	if (mTime < 0.0f)
	{
		mTime = 0.0f;
		mOutFlag = false;
		mNowFlag = false;
	}
}

