//--------------------------------------------------------------------------------------
// File		: PlayScene.h
//
//　メインゲームのシステムを制御,実行
// 
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "NecromaLib/GameData/Camera.h"
#include "Camera/MoveCamera.h"

// ゲームのオブジェクト
#include "Field/FieldObjectManager.h"
#include "Mouse/MousePointer.h"
#include "AlchemicalMachine/AlchemicalMachineManager.h"
#include "Enemy/EnemyManager.h"

// ミッション
#include "Scenes/PlayScene/Mission/MissionManager.h"

// UI関連
#include "Scenes/PlayScene/UI/BaseLv.h"
#include "Scenes/PlayScene/UI/Gauge.h"
#include "Scenes/PlayScene/Tutorial/OperationInstructions.h"
#include "Scenes/PlayScene/Tutorial/TutorialManager.h"
#include "Scenes/PlayScene/Tutorial/Explanation.h"

// 自作ポストプロセス
#include "Scenes/Commons/MyPostProcess.h"


class MouseStaging;

class PlayScene : public SceneObj
{
public:
	// コンストラクタ
	PlayScene();

	// デストラクタ
	~PlayScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update() override;

	// 描画
	void Draw() override;

	// 深度描画
	void DrawShadow();

	// UI系描画
	void DrawUI() override;

	// 終了処理
	void Finalize() override;

	void EnemyToAMMachine();
	void EnemyToPlayerBase();
	void EnemyToBullet();

public:

	// フィールドマネージャークラス
	FieldObjectManager*			GetFieldManager()				const { return m_fieldManager.get(); }
	//マウスポインタークラス
	MousePointer*				GetMousePointer()				const { return m_mousePointer.get(); }
	//マシンマネージャークラス
	AlchemicalMachineManager*	GetAlchemicalMachineManager()	const { return m_AM_Manager.get(); }
	//エネミーマネージャークラス
	EnemyManager*				GetEnemyManager()				const { return m_enemyManager.get();}
	//ミッションマネージャークラス
	MissionManager*				GetMissionManager()				const { return m_missionManager.get();}
	//ゲージクラス
	Gauge*						GetGauge()						const { return m_resourceGauge.get(); }
	//Lvクラス
	BaseLv*						GetBaseLv()						const { return m_baseLv.get(); }
	//説明クラス
	Explanation*				GetExplanation()				const { return m_explanation.get(); }
	//倍速ボタンクラス
	SelectionBox*				GetSpeedUpButton()				const { return m_doubleSpeedButton.get(); }
	// カメラクラス
	MoveCamera*					GetMoveCamrera()				const { return m_moveCamera.get(); }

private:

	// 特定のUI群のアルファ値を変え、無効化する
	void TransparentUI(float transparentVal);

private:

	// フィールドに存在するオブジェクトを管理
	std::unique_ptr<FieldObjectManager>			m_fieldManager;

	// マウスポインターにかかる処理
	std::unique_ptr<MousePointer>				m_mousePointer;

	// マシンユニット/マシンに関連するUIの管理
	std::unique_ptr<AlchemicalMachineManager>	m_AM_Manager;

	// カメラに対する処理
	std::unique_ptr<MoveCamera>					m_moveCamera;

	// エネミーを管理
	std::unique_ptr<EnemyManager>				m_enemyManager;

	// ミッションを管理
	std::unique_ptr<MissionManager>				m_missionManager;

	// HP,MP,クリスタルのゲージ
	std::unique_ptr<Gauge>						m_resourceGauge;

	// EXP表示に使用
	std::unique_ptr<BaseLv>						m_baseLv;

	// チュートリアルクラス
	std::unique_ptr<OperationInstructions>		m_operationInstructions;

	// チュートリアルマネージャークラス
	std::unique_ptr<TutorialManager>			m_tutorialManager;

	// マウス操作説明クラス
	std::unique_ptr<Explanation>				m_explanation;

	// 天球モデル
	std::unique_ptr<Model>						m_skySphere;

	// 倍速ボタン
	std::unique_ptr<SelectionBox>				m_doubleSpeedButton; 

	// マウスが範囲に入っているか否かを検出する当たり判定クラス
	std::unique_ptr<SelectionBox> m_collider;
	// UIが半透明化するまでの時間
	float m_uiTransparentTime;

	// ステージ番号
	int m_stageNumber;
	// 最大ステージ番号
	int m_maxWaveNum;
	// 倍速倍率
	int m_doubleSpeedNum;

	// レンダーテクスチャ
	std::unique_ptr<DX::RenderTexture> m_offscreenRT;

	// ポストプロセス
	std::unique_ptr<MyPostProcess> m_postProcess;

};