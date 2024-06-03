#include "pch.h"
#include "MissionManager.h"

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"
#include "Scenes/PlayScene/Enemy/EnemyManager.h"
#include "Scenes/PlayScene/Field/FieldObjectManager.h"
#include "Scenes/PlayScene/UI/Number.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/PlayScene/Mission/ClearVeil.h"

#include "Scenes/Commons/DrawArrow.h"

#include "Scenes/SelectScene/MissionRender.h"

#include "Scenes/TitleScene/TitleLogo/Veil.h"

#include "Scenes/Commons/DrawTimer.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "NecromaLib/GameData/UserInterfase.h"
#include "NecromaLib/GameData/JsonLoder.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/GameData/Easing.h"

#define TEX_W 320
#define TEX_H 48

#define TEX_SMALL 64

// ステージクリア/失敗時のアニメーションスピード
#define CLEAR_ANIM_SPEED 2.8f

MissionManager::MissionManager() :
	m_allClearFlag(),
	m_missionNum(),
	m_missionSituation(),
	m_gameTimer(),
	m_failureFlag(),
	m_baseHP(),
	m_lastWave(),
	m_wave(1),
	m_nextWaveFlag(),
	m_closeAnimation(),
	m_closeMission(false),
	m_closeButton(),
	m_waveTimer(),
	m_endlesFlag()
{
}

MissionManager::~MissionManager()
{
}

void MissionManager::Initialize(int waveNum)
{
	// ステージをエンドレスモードにする
	if (waveNum <= -1)
	{
		m_endlesFlag = true;
	}

	//　====================[　デバイスと画面サイズの確保　]
	auto device = ShareData::GetInstance().GetDeviceResources();
	int width	= device->GetOutputSize().right;
	int height	= device->GetOutputSize().bottom;

	UI_Data ui_data = ShareJsonData::GetInstance().GetUIData("MissionOffset");

	//　====================[　マウスが周辺に位置するかを取得　]
	m_collider = std::make_unique<SelectionBox>(ui_data.pos, SimpleMath::Vector2(ui_data.option["COLLIDER_X"], ui_data.option["COLLIDER_Y"]));
	m_collider->Initialize();
	m_collider->SetRect(RECT{ 0,0,1,1 });
	m_collider->SetWorldInfluenceFlag(false);

	//　====================[　ステージ情報の再度読み込み　]
	ReloadWave();

	//　====================[　時間描画クラスの設定　]
	ui_data = ShareJsonData::GetInstance().GetUIData("MissionTimer");
	m_timeRender = std::make_unique<DrawTimer>(ui_data.pos, ui_data.rage);

	//　====================[　Wave数描画　]
	ui_data = ShareJsonData::GetInstance().GetUIData("MissionNowWave");
	m_waveRender	= std::make_unique<Number>(ui_data.pos, ui_data.rage);
	m_waveRender	->SetColor(SimpleMath::Color(Colors::White));

	ui_data = ShareJsonData::GetInstance().GetUIData("MissionMaxWave");
	m_maxWaveRender = std::make_unique<Number>(ui_data.pos, ui_data.rage);
	m_maxWaveRender	->SetColor(SimpleMath::Color(Colors::White));

	//　====================[　ミッションクラスの設定　]
	ui_data = ShareJsonData::GetInstance().GetUIData("MissionText");
	m_missionRender = std::make_unique<MissionRender>(ui_data.pos, ui_data.rage);
	//　　|=>　位置の再登録
	m_collider->SetSavePos(ui_data.pos);

	//　====================[　ミッションの開閉を行う矢印クラスの設定　]
	ui_data = ShareJsonData::GetInstance().GetUIData("MissionArrow");
	m_closeButton = std::make_unique<DrawArrow>(ui_data.pos, ui_data.rage, (int)ui_data.option["Open"]);
	m_closeButton->Initialize();

	//　====================[　ステージクリア時演出クラスの設定　]
	m_backVeil = std::make_unique<Veil>(3);
	m_backVeil->Create(L"Resources/Textures/Fade.png");
	m_backVeil->LoadShaderFile(L"Veil");
	m_backVeil->SetWindowSize(width, height);
	m_backVeil->SetColor(SimpleMath::Color(0.4f, 0.4f, 0.4f, 0.5f));
	m_backVeil->SetScale(SimpleMath::Vector2((float)width, (float)height / 5.0f));
	m_backVeil->SetPosition(SimpleMath::Vector2(0.0f, height / 2.5f));

	//　====================[　クリア時の演出　]
	m_clearVeil = std::make_unique<ClearVeil>(waveNum);

	//　====================[　ステージ失敗成功時のアニメーション用変数　]
	m_clearAnimation.max = 2.0f;

	//　====================[　エンドレスモード用のミッション生成クラス　]
	m_endlessMode = std::make_unique<EndlessMode>();
	m_endlessMode->Initialize();

}

void MissionManager::Update(AlchemicalMachineManager* pAlchemicalManager, EnemyManager* pEnemyManager, FieldObjectManager* pFieldManager)
{
	auto pDeltaT = &DeltaTime::GetInstance();
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	//　====================[　最終ウェーブであるか否かを取得　]
	m_lastWave = pSJD.GetStageData().lastWave;

	//　　|=>　拠点のHPを取得する
	m_baseHP = (int)DataManager::GetInstance()->GetNowBaseHP();

	//　====================[　UIの位置を決定　]
	UI_Data ui_dataText	 = pSJD.GetUIData("MissionText");
	UI_Data ui_dataArrow = pSJD.GetUIData("MissionArrow");
	UI_Data ui_dataTimer = pSJD.GetUIData("MissionTimer");
	UI_Data ui_dataMaxWave = pSJD.GetUIData("MissionMaxWave");
	UI_Data ui_dataNowWave = pSJD.GetUIData("MissionNowWave");
	SimpleMath::Vector2 missionPos = SimpleMath::Vector2(ui_dataText.pos.x - Easing::EaseInCubic(0, ui_dataText.option["CloseVal"], m_closeAnimation),
														 ui_dataText.pos.y);

	//　====================[　UIの移動処理　]
	{
		m_missionRender->SetPos(missionPos);
		m_timeRender->SetPos(SimpleMath::Vector2(missionPos.x + ui_dataTimer.pos.x, ui_dataTimer.pos.y));
		m_waveRender->SetPosition(SimpleMath::Vector2(missionPos.x + ui_dataNowWave.pos.x, ui_dataNowWave.pos.y));
		m_maxWaveRender->SetPosition(SimpleMath::Vector2(missionPos.x + ui_dataMaxWave.pos.x, ui_dataMaxWave.pos.y));
		m_closeButton->SetSavePos(SimpleMath::Vector2(missionPos.x + (ui_dataArrow.pos.x * 1.5f), ui_dataArrow.pos.y));
	}

	//　====================[　開閉矢印の更新　]
	m_closeButton->HitMouse();

	//　====================[　開閉矢印の向きを変える処理　]
	if (m_closeButton->SelectionMouse())
	{
		m_closeButton->SetDirection((int)ui_dataArrow.option["Close"]);
		m_closeAnimation += pDeltaT->GetNomalDeltaTime();
	}
	else
	{
		m_closeAnimation -= pDeltaT->GetNomalDeltaTime();
		m_closeButton->SetDirection((int)ui_dataArrow.option["Open"]);
	}


	//　====================[　マシンが設置された際の処理　]
	if (pAlchemicalManager->SpawnMachineNotification()		!= MACHINE_TYPE::NONE) 						MachineMission(pAlchemicalManager);

	//　====================[　マシンが錬金された　]
	if (pAlchemicalManager->GetMachineSelect()->get()->GetManufacturingFlag() != MACHINE_TYPE::NONE)	AlchemiMission(pAlchemicalManager);


	//　====================[　エネミーが倒された　]
	if (pEnemyManager->GetKnokDownEnemyType()				!= ENEMY_TYPE::ENMEY_NONE) 					EnemyMission(pEnemyManager);

	// 拠点Lvの処理
	if (m_missonCondition[MISSION_TYPE::RESOURCE].size() > 0)											BaseLvMission(pFieldManager->GetPlayerBase()->GetBaseLv());

	// 時間制限の処理
	if (m_missonCondition[MISSION_TYPE::TIMER].size() > 0)												TimerMission();

	// リソースに変化があった際に通す
	if (pAlchemicalManager->GetPulsMpVal() > 0.0f)		ResourceMission(pAlchemicalManager);

	// クリスタルリソースに変化があった際に通す
	if (pAlchemicalManager->GetPulsCrystalVal() > 0.0f)	ResourceMission(pAlchemicalManager);

	// クリア、または失敗時にアニメーションを回す
	if ((m_missionNum <= m_missionSituation) || MissionUnable())
	{
		// 最後のWaveか失敗ならばシーン遷移開始とする
		if (m_lastWave || MissionUnable())
		{
			m_backVeil->Update();
			m_clearAnimation += 0.02f;
		}
		else
		{
			m_nextWaveFlag = true;
		}
	}

	//　====================[　WAVEクリア時に流す処理(最後のWaveの時は流さない)　] 
	m_clearVeil->Update(m_nextWaveFlag && !m_lastWave);

	//　　|=>　次のWaveに移行
	if (m_clearVeil->NextWave())
	{
		// Waveを加算する
		m_wave++;

		if(m_endlesFlag)
		{ 
			// エンドレス用にランダム生成したステージをステージ情報とする
			m_endlessMode->CreateStage(m_wave);

		}
		else
		{
			// 次のWaveを読み込む
			ShareJsonData::GetInstance().LoadingJsonFile_Stage(DataManager::GetInstance()->GetStageNum(), m_wave);
		}
	}
}

void MissionManager::TimerUpdate()
{
	auto pDeltaT = &DeltaTime::GetInstance();
	m_gameTimer += pDeltaT->GetDeltaTime();
	m_waveTimer += pDeltaT->GetDeltaTime();

	m_timeRender->Update(m_gameTimer);

}

void MissionManager::Render(int maxwaveNum)
{

	ShareData& pSD = ShareData::GetInstance();

	//　====================[　デバイスと画面サイズの確保　]
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);

	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// ミッション内容の描画
	m_missionRender->Render_MachineMission	(m_missonCondition[MISSION_TYPE::SPAWN]);
	m_missionRender->Render_AlchemiMission	(m_missonCondition[MISSION_TYPE::ALCHEMI]);
	m_missionRender->Render_EnemyMission	(m_missonCondition[MISSION_TYPE::ENEMY_KILL]);
	m_missionRender->Render_BaseLvMission	(m_missonCondition[MISSION_TYPE::BASELV]);
	m_missionRender->Render_TimerMission	(m_missonCondition[MISSION_TYPE::TIMER]);
	m_missionRender->Render_ResourceMission (m_missonCondition[MISSION_TYPE::RESOURCE]);

	m_missionRender->LineReset();

	// 経過時間の描画
	m_timeRender->TimerDraw();

	//　====================[　ステージクリア/失敗時の演出　]
	if ((m_missionNum <= m_missionSituation) || MissionUnable())
	{
		m_backVeil->Render();

		SpriteLoder& pSL = SpriteLoder::GetInstance();

		int filed = (int)MissionUnable();

		RECT rect = SpriteCutter(TEX_W, TEX_H, filed, 0);

		pSD.GetSpriteBatch()->Draw(pSL.GetStageClearTextTexture().Get(),
			SimpleMath::Vector2(width / 2.0f, height / 2.0f - (m_clearAnimation * CLEAR_ANIM_SPEED)), &rect,
			SimpleMath::Color(1.0f, 1.0f, 1.0f, m_clearAnimation),
			0.0f, SimpleMath::Vector2(TEX_W / 2.0f, TEX_H / 2.0f), 2.0f);

	}

	//====================[「/」描画 ]
	//「/」描画
	RECT slashRect = SpriteCutter(TEX_SMALL, TEX_SMALL, 0, 0);
	pSD.GetSpriteBatch()->Draw(SpriteLoder::GetInstance().GetSymbolTexture().Get(),
		m_waveRender->GetPosition() + SimpleMath::Vector2(TEX_SMALL * m_waveRender->GetRage().x,0.0f),
		&slashRect,
		Colors::White,
		0.0f,
		DirectX::XMFLOAT2(TEX_SMALL / 2, TEX_SMALL / 2),
		m_waveRender->GetRage());

	pSD.GetSpriteBatch()->End();

	m_closeButton		->Draw();

	m_clearVeil			->Render(m_wave);

	//　====================[　Wave数描画　]
	m_maxWaveRender	->SetNumber(maxwaveNum);
	m_maxWaveRender	->Render();

	m_waveRender	->SetNumber(m_wave);
	m_waveRender	->Render();
}

void MissionManager::ReloadWave()
{

	auto pSJD = &ShareJsonData::GetInstance();

	// ミッションの達成度
	m_missionSituation = 0;

	// ウェーブ内の時間をリセットする
	m_waveTimer = 0;

	// 取得した情報をコピーする
	for (int i = 0; i < MISSION_TYPE::MISSION_NUM; i++)
	{
		m_missonCondition[i] = pSJD->GetStageData().condition[i];

		if (m_missonCondition[i][0].value <= 0)		m_missionSituation++;
	}

	// それぞれの内容の合計値を得る
	m_missionNum = 
		(int)m_missonCondition[MISSION_TYPE::SPAWN].size()		+
		(int)m_missonCondition[MISSION_TYPE::ALCHEMI].size()	+
		(int)m_missonCondition[MISSION_TYPE::ENEMY_KILL].size()	+
		(int)m_missonCondition[MISSION_TYPE::RESOURCE].size()	+
		(int)m_missonCondition[MISSION_TYPE::BASELV].size()		+
		(int)m_missonCondition[MISSION_TYPE::TIMER].size();

	// ミッションを全てクリアしたフラグを元に戻す
	m_allClearFlag = false;

	// ウェーブが進んだため、要求をfalseにする
	m_nextWaveFlag = false;
}

bool MissionManager::MissionComplete()
{
	return (m_missionNum <= m_missionSituation) && m_clearAnimation.MaxCheck();
}

bool MissionManager::MissionUnable()
{
	return m_failureFlag || m_baseHP <= 0;
}

bool MissionManager::MissionFailure()
{
	return MissionUnable() && m_clearAnimation.MaxCheck();
}

int MissionManager::GetStartTimer()
{
	return m_missonCondition[MISSION_TYPE::TIMER][0].progress;
}

void MissionManager::TransparentUI(float transparentVal)
{

	m_missionRender	->SetAlpha(transparentVal);
	m_closeButton	->SetColor(SimpleMath::Color(m_closeButton		->GetColorRGB(), transparentVal));
	m_timeRender	->SetColor(SimpleMath::Color(m_timeRender		->GetColorRGB(), transparentVal));
	m_waveRender	->SetColor(SimpleMath::Color(m_waveRender		->GetColorRGB(), transparentVal));
	m_maxWaveRender	->SetColor(SimpleMath::Color(m_maxWaveRender	->GetColorRGB(), transparentVal));
}

void MissionManager::MachineMission(AlchemicalMachineManager* alchemicalManager)
{
	// 対応する条件をTrueにする：設置関連
	for (int i = 0; i < m_missonCondition[MISSION_TYPE::SPAWN].size(); i++)
	{
		// ミッションの内容と同じならば処理を通す 既にミッションが済んでいる場合は飛ばす
		if (ChangeData::ChangeMachine(m_missonCondition[MISSION_TYPE::SPAWN][i].condition) ==
			alchemicalManager->SpawnMachineNotification() &&
			m_missonCondition[MISSION_TYPE::SPAWN][i].progress < m_missonCondition[MISSION_TYPE::SPAWN][i].value)
		{
			m_missonCondition[MISSION_TYPE::SPAWN][i].progress++;

			// 攻略完了
			if (m_missonCondition[MISSION_TYPE::SPAWN][i].progress >= m_missonCondition[MISSION_TYPE::SPAWN][i].value)
			{
				m_missionSituation++;
			}
		}
	}
}

void MissionManager::AlchemiMission(AlchemicalMachineManager* alchemicalManager)
{

	// 対応する条件をTrueにする:錬金関連
	for (int i = 0; i < m_missonCondition[MISSION_TYPE::ALCHEMI].size(); i++)
	{
		// ミッションの内容と同じならば処理を通す 既にミッションが済んでいる場合は飛ばす
		if (ChangeData::ChangeMachine(m_missonCondition[MISSION_TYPE::ALCHEMI][i].condition)
			== alchemicalManager->GetMachineSelect()->get()->GetSelectMachineType()
			&& m_missonCondition[MISSION_TYPE::ALCHEMI][i].progress < m_missonCondition[MISSION_TYPE::ALCHEMI][i].value)
		{

			m_missonCondition[MISSION_TYPE::ALCHEMI][i].progress++;

			// 攻略完了
			if (m_missonCondition[MISSION_TYPE::ALCHEMI][i].progress >= m_missonCondition[MISSION_TYPE::ALCHEMI][i].value)
			{
				m_missionSituation++;
			}
		}
	}

}

void MissionManager::ResourceMission(AlchemicalMachineManager* alchemicalManager)
{

	// 対応する条件をTrueにする：リソース条件
	for (int i = 0; i < m_missonCondition[MISSION_TYPE::RESOURCE].size(); i++)
	{
		if (m_missonCondition[MISSION_TYPE::RESOURCE][i].progress >= m_missonCondition[MISSION_TYPE::RESOURCE][i].value) continue;

		if (m_missonCondition[MISSION_TYPE::RESOURCE][i].condition == "MP")		m_missonCondition[MISSION_TYPE::RESOURCE][i].progress += alchemicalManager->GetPulsMpVal();

		if (m_missonCondition[MISSION_TYPE::RESOURCE][i].condition == "Crystal")	m_missonCondition[MISSION_TYPE::RESOURCE][i].progress += alchemicalManager->GetPulsCrystalVal();

		if (m_missonCondition[MISSION_TYPE::RESOURCE][i].progress >= m_missonCondition[MISSION_TYPE::RESOURCE][i].value)
		{
			m_missionSituation++;
		}

	}
}

void MissionManager::EnemyMission(EnemyManager* enemyManager)
{

	// 対応する条件をTrueにする
	for (int i = 0; i < m_missonCondition[MISSION_TYPE::ENEMY_KILL].size(); i++)
	{
		// ミッションの内容と同じならば処理を通す 既にミッションが済んでいる場合は飛ばす
		if (ChangeData::ChangeEnemy(m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].condition) ==
			enemyManager->GetKnokDownEnemyType() &&
			m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].progress < m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].value)
		{
			// 同一フレーム内に複数対敵がやられたとしても対応可能にする
			m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].progress += enemyManager->GetKnokDownEnemyFlag();

			// 攻略完了
			if (m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].progress >= m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].value)
			{
				m_missionSituation++;
			}
		}

		// 全対応の場合
		if (m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].condition == "All")
		{
			// 同一フレーム内に複数対敵がやられたとしても対応可能にする
			m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].progress += enemyManager->GetKnokDownEnemyFlag();

			// 攻略完了
			if (m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].progress >= m_missonCondition[MISSION_TYPE::ENEMY_KILL][i].value)
			{
				m_missionSituation++;
			}
		}

	}

}

void MissionManager::BaseLvMission(int baseLv)
{
	// 拠点Lvが目標値に達するまで更新
	if (m_missonCondition[MISSION_TYPE::BASELV][0].progress < m_missonCondition[MISSION_TYPE::BASELV][0].value)
	{
		m_missonCondition[MISSION_TYPE::BASELV][0].progress = baseLv;

		if (m_missonCondition[MISSION_TYPE::BASELV][0].progress >= m_missonCondition[MISSION_TYPE::BASELV][0].value)
		{
			m_missionSituation++;
		}
	}

}

void MissionManager::TimerMission()
{

	// 生き残れば勝利系タイマー
	if (m_missonCondition[MISSION_TYPE::TIMER][0].progress < m_missonCondition[MISSION_TYPE::TIMER][0].value)
	{
		// 毎秒進行度を上げる
		m_missonCondition[MISSION_TYPE::TIMER][0].progress = (int)m_waveTimer;

		if (m_missonCondition[MISSION_TYPE::TIMER][0].progress >= m_missonCondition[MISSION_TYPE::TIMER][0].value)
		{

			if (m_missonCondition[MISSION_TYPE::TIMER][0].condition == "Standerd") m_missionSituation++;
			if (m_missonCondition[MISSION_TYPE::TIMER][0].condition == "Limit") m_failureFlag = true;
			
		}
	}
}

bool MissionManager::NextWaveFlag()
{
	return m_nextWaveFlag && m_clearVeil->NextWave();
}