#pragma once

#include "StepTimer.h"
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

class Fade
{

private:

	int   mScreenW;
	int   mScreenH;

	float mSpeed;
	float mTime;

	bool mInFlag,mNowFlag,mOutFlag;

	std::unique_ptr<DirectX::SpriteBatch>   mSpriteBatch;
	//std::unique_ptr<DirectX::SpriteFont>    mSpriteFont;
	//std::unique_ptr<DirectX::CommonStates>  mCommonState;

	// フェードの際に使うテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mFadeTexture;


	//--	シングルトンに設定	--//

	Fade();

	static Fade* instance;

public:
	~Fade() = default;


	//--	シングルトンに設定	--//

	//	コピーコンストラクタのデリート
	Fade(const Fade&) = delete;
	Fade& operator=(const Fade&) = delete;

	//	インスタンスの確保
	static Fade* GetInstance() { return instance; }

	//	インスタンスの生成
	static void Create() { if (instance == nullptr) instance = new Fade; }

	//	インスタンスの削除
	static void Destroy()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}


	//　関数

	void Initialize(int screenWidth, int screenHeight);
	void Render();
	void Finalize();

	void SetFadeSpeed(float speed) { mSpeed = speed; }

	void Update(DX::StepTimer const& timer);
	void FadeInUpdate(DX::StepTimer const& timer);
	void FadeOutUpdate(DX::StepTimer const& timer);

	void StartFadeIn() { mInFlag = true; }
	void StartFadeOut() { mOutFlag = true; }

	bool ProcessCheckFadeIn() { return mInFlag; }
	bool ProcessCheckFadeOut() { return mOutFlag; }
	bool ProcessCheckFadeNow() { return mNowFlag; }

};