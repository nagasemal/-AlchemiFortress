#include "pch.h"
#include "ClearVeil.h"

#include "NecromaLib/GameData/UserInterfase.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"

#define ANIMATION_SPEED_1 1.25f		// 左右から出現する幕のアニメーションスピード
#define ANIMATION_SPEED_2 1.5f		// 上下に移動する幕のアニメーションスピード
#define ANIMATION_SPEED_3 2.0f		// Wave数を表示,撤退させるアニメーションスピード

#define VEIL_SIZE_Y 32.0f			// 上下から現れる幕のサイズ
#define WAVE_SIZE 0.06f				// Wave数表示画像のサイズ
#define WAVE_UP_VAL 10.0f			// 演出終了時,上方へ撤退する際の大きさ

#define WAVE_ANIM_MIDDLE 0.2f

ClearVeil::ClearVeil(int waveNum)
{
	//　====================[　デバイスと画面サイズの確保　]
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);

	//　====================[　Waveクリア時演出クラスの設定　]
	//　　|=>　上部
	m_nextWave_Top = std::make_unique<UserInterface>();
	m_nextWave_Top->Create(device,
		L"Resources/Textures/WaveClear_Top.png",
		SimpleMath::Vector2(width / 2.0f, 0.0f),
		SimpleMath::Vector2(1.0f, 0.5f),
		UserInterface::ANCHOR::MIDDLE_CENTER);
	m_nextWave_Top->SetColor(Colors::Aqua);
	//　　|=>　下部
	m_nextWave_Bottom = std::make_unique<UserInterface>();
	m_nextWave_Bottom->Create(device,
		L"Resources/Textures/WaveClear_Bottom.png",
		SimpleMath::Vector2(width / 2.0f, height),
		SimpleMath::Vector2(1.0f, 0.5f),
		UserInterface::ANCHOR::MIDDLE_CENTER);
	m_nextWave_Bottom->SetColor(Colors::Aqua);
	m_nextWave_Bottom->SetSharpenW(true);

	// アニメーションデータの設定
	m_waveAnimation_first	= 0.0f;
	m_waveAnimation_second	= 0.0f;
	for (int i = 0; i < waveNum; i++)
	{
		AnimationData animData = 0.0f;
		animData.middle = WAVE_ANIM_MIDDLE;
		m_waveAnimation_waves.push_back(animData);
	}


}

ClearVeil::~ClearVeil()
{
}

void ClearVeil::Update(bool startFlag)
{
	//　====================[　デバイスと画面サイズの確保　]
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);

	//　====================[　アニメーション.1　]
	//　　|=>　上下のラベルを表示
	if (startFlag)
	{
		m_waveAnimation_first += DeltaTime::GetInstance().GetDeltaTime() * ANIMATION_SPEED_1;

		m_nextWave_Top->SetSharpenW(true);
		m_nextWave_Bottom->SetSharpenW(false);

	}
	else
	{
		m_waveAnimation_first -= DeltaTime::GetInstance().GetDeltaTime() * ANIMATION_SPEED_1;

		m_nextWave_Top->SetSharpenW(false);
		m_nextWave_Bottom->SetSharpenW(true);
	}

	//　====================[　アニメーション.2　]
	//　　|=>　表示部分を多く見せる
	if (m_waveAnimation_first.MaxCheck() && !m_waveAnimation_second.MaxCheck())
	{
		m_waveAnimation_second += DeltaTime::GetInstance().GetDeltaTime() * ANIMATION_SPEED_2;
	}
	//　　|=>　リセット
	if (m_waveAnimation_first.MinCheck())
	{
		m_waveAnimation_second = 0.0f;

		for (int i = 0; i < m_waveAnimation_waves.size();i++)
		{
			m_waveAnimation_waves[i] -= DeltaTime::GetInstance().GetDeltaTime() * ANIMATION_SPEED_3;
		}
	}

	//　====================[　アニメーション.3　]
	//　　|=>　Wave数を視覚的に表現
	if (m_waveAnimation_second.MiddleFlag())
	{
		m_waveAnimation_waves[0] += DeltaTime::GetInstance().GetDeltaTime() * ANIMATION_SPEED_3;

		//　　|=>　前の要素の時間変数が半分の値を過ぎたら更新を始める
		for (int i = 1; i < m_waveAnimation_waves.size();)
		{
			if (m_waveAnimation_waves[i - 1].MiddleFlag())
			{
				m_waveAnimation_waves[i] += DeltaTime::GetInstance().GetDeltaTime() * ANIMATION_SPEED_3;
			}

			i++;
		}
	}

	//　====================[　表示割合を設定　]
	m_nextWave_Top		->SetRenderRatio(Easing::EaseInOutCirc(0.0f, 1.0f, m_waveAnimation_first));
	m_nextWave_Bottom	->SetRenderRatio(Easing::EaseInOutCirc(0.0f, 1.0f, m_waveAnimation_first));

	//　====================[　表示位置を設定　]
	m_nextWave_Top		->SetPosition(SimpleMath::Vector2(width / 2.0f,			 Easing::EaseOutCubic(0.0f, VEIL_SIZE_Y, m_waveAnimation_second)));
	m_nextWave_Bottom	->SetPosition(SimpleMath::Vector2(width / 2.0f, height - Easing::EaseOutCubic(0.0f, VEIL_SIZE_Y, m_waveAnimation_second)));

}

void ClearVeil::Render(int nowWave)
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();

	//　====================[　デバイスと画面サイズの確保　]
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);

	//　====================[　描画　]
	m_nextWave_Top->Render();
	m_nextWave_Bottom->Render();

	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	//　====================[　残りWave数を描画　]
	for (int i = 0; i < m_waveAnimation_waves.size(); i++)
	{
		SpriteLoder::TextureData texData = pSL.GetMachineMagicCircleTexture(0);

		// 横方向へ均等に並べる
		float posX_shift = ((float)(m_waveAnimation_waves.size() - 1) / 2.0f - (float)i) * ((float)texData.width * WAVE_SIZE);

		// 演出終了時に上に登る変数
		float posY_shift = Easing::EaseInOutCirc(0.0f, WAVE_UP_VAL, m_waveAnimation_first);

		// 色情報
		SimpleMath::Color		color = SimpleMath::Color(Colors::White);

		// クリアしたWaveならば色を塗る
		if (nowWave - 1 >= i)	color = SimpleMath::Color(Colors::Aqua);

		color.A(Easing::EaseInOutSine(0.0f, 1.0f, m_waveAnimation_waves[i]));

		RECT rect = { 0,0,texData.width,texData.height };
		pSD.GetSpriteBatch()->Draw(texData.tex.Get(),
			SimpleMath::Vector2(width / 2.0f - posX_shift, height / 2.0f + posY_shift), &rect,
			color,
			1.0f - m_waveAnimation_waves[i],
			SimpleMath::Vector2(texData.width / 2.0f, texData.height / 2.0f), WAVE_SIZE);
	}

	pSD.GetSpriteBatch()->End();
}

bool ClearVeil::NextWave()
{
	int waveSize = (int)m_waveAnimation_waves.size() - 1;

	return m_waveAnimation_second.MaxCheck() && m_waveAnimation_first.MaxCheck() && m_waveAnimation_waves[waveSize].MaxCheck();
}