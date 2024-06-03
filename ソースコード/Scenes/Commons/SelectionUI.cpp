#include "pch.h"
#include "SelectionUI.h"

#include "NecromaLib/GameData/UserInterfase.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/SoundData.h"

#include <random>

// 画像サイズの縦横の大きさ
#define SPRITE_RAGE 64

// 失敗時振動の大きさ
#define SHAKE_RAGE 2.0f

SelectionUI::SelectionUI() :
	m_selectFlag(),
	m_hitMouseFlag(),
	m_keySelectFlag(),
	m_worldInfluenceFlag(true),
	m_layer(),
	m_activeFlag(true),
	m_rect{ 0,0,SPRITE_RAGE,SPRITE_RAGE },
	m_vibrationTime(),
	m_holdTime(),
	m_keys()
{
}

SelectionUI::~SelectionUI()
{
}

void SelectionUI::Initialize()
{
	m_boxColor = Colors::White;
}

void SelectionUI::Finalize()
{
}

bool SelectionUI::HitMouse(bool layerCheck)
{
	InputSupport& pINP = InputSupport::GetInstance();

	// 透明(見えない)場合はfalseを返す
	if (m_boxColor.A() < 0.0f)
	{
		m_hitMouseFlag = false;
		return false;
	}

	// 判定を取得する
	m_hitMouseFlag = HitObject_RageSet(pINP.GetMousePosScreen(), 
									   static_cast<float>(m_rect.right),
									   static_cast<float>(m_rect.bottom),
									   m_data.rage) || m_keySelectFlag;

	m_keySelectFlag = false;

	for (auto& key : m_keys)
	{
		if (!m_keySelectFlag)
		{
			m_keySelectFlag = pINP.GetKeybordState().IsKeyPressed(key);
		}
	}

	// マウスがワールド空間に影響を及ぼさない様にする
	if (m_hitMouseFlag && m_worldInfluenceFlag) pINP.HitUI();

	// 震わせる
	if (m_vibrationTime > 0.0f)
	{
		std::uniform_real_distribution<> dist_enemyType(-m_vibrationTime, m_vibrationTime);
		std::random_device rd;
		float vibrationRand = static_cast<float>(dist_enemyType(rd)) * SHAKE_RAGE;

		m_vibrationTime -= DeltaTime::GetInstance().GetNomalDeltaTime();

		m_data.pos = SimpleMath::Vector2(m_saveData.pos.x + sinf(vibrationRand) , m_saveData.pos.y + cosf(vibrationRand));

	}
	else
	{
		m_vibrationTime = 0.0f;
		m_data.pos = m_saveData.pos;
	}


	// 触れている間はワールド空間座標に影響を及ぼさないようにする
	// layerチェック状態ならば追加でチェックを行う
	if (m_hitMouseFlag && layerCheck)
	{
		// 自身の方が大きければレイヤー数を更新する そうでなければ処理を通さない
		if (m_layer >= (int)pINP.GetLayer())	pINP.SetLayer(m_layer);
		else return m_hitMouseFlag = false;

	}

	return m_hitMouseFlag;
}

bool SelectionUI::ClickMouse()
{
	InputSupport& pINP = InputSupport::GetInstance();

	if ((m_hitMouseFlag && pINP.LeftButton_Press()) || m_keySelectFlag)
	{
		// アクティブ時
		if (m_activeFlag)
		{
			SoundData::GetInstance().PlaySystemSE_Auto(XACT_WAVEBANK_SYSTEMSE_BUTTOMPUSH);
			return true;
		}
		else
		{
			SoundData::GetInstance().PlaySystemSE_Auto(XACT_WAVEBANK_SYSTEMSE_POPTEXT);

			m_vibrationTime = 1.0f;

			return false;
		}
	}

	return false;
}

bool SelectionUI::SelectionMouse()
{
	//　====================[　UIが押された際のON・OFF処理　]
	//　　|=>　ON
	if (ClickMouse() && !m_selectFlag)
	{
		m_selectFlag = true;
		return m_selectFlag;
	}
	//　　|=>　OFF
	if (ClickMouse() && m_selectFlag)
	{
		m_selectFlag = false;
	}

	return m_selectFlag;
}

bool SelectionUI::HoldMouse()
{
	InputSupport& pINP = InputSupport::GetInstance();
	bool leftFlag = pINP.GetMouseState().leftButton;

	if (m_hitMouseFlag && leftFlag) m_holdTime += DeltaTime::GetInstance().GetDeltaTime();
	else m_holdTime = 0.0f;

	return m_holdTime >= 1.0f;
}

bool SelectionUI::ExitMouse()
{
	InputSupport& pINP = InputSupport::GetInstance();
	bool exitFlag = false;

	if (m_hitMouseFlag)
	{
		exitFlag = !HitObject_RageSet(pINP.GetMousePosScreen(), SPRITE_RAGE, SPRITE_RAGE, m_data.rage);
	}

	return exitFlag;
}


