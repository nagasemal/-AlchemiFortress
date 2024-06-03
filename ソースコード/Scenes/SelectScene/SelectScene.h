//--------------------------------------------------------------------------------------
// File		: SelectScene.h
//
//　ステージセレクトのシステムを制御,実行
// 
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "NecromaLib/GameData/Camera.h"

#include "Scenes/Commons/DrawMachine.h"
#include "Scenes/SelectScene/SelectCamera.h"
#include "Scenes/SelectScene/MissionRender.h"

class DrawArrow;
class SelectionBox;
class UIKeyControl;
class Number;

class SelectScene : public SceneObj
{
public:
	// コンストラクタ
	SelectScene();

	// デストラクタ
	~SelectScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update() override;

	// 描画
	void Draw() override;

	// UI系描画
	void DrawUI() override;

	// 終了処理
	void Finalize() override;

	int GetStageNumber() const { return m_selectStageNumber;}
	void SetStageNumber(int number) { m_selectStageNumber = number;}

private:

	// 選択したステージの番号
	int m_selectStageNumber;

	std::unique_ptr<DirectX::Model>		m_skySphere;

	// セレクトシーン内のカメラ
	std::unique_ptr<SelectCamera> m_selectCamera;

	// マシンの描画
	std::unique_ptr<DrawMachine> m_machineDraw;

	// ミッションの描画
	std::unique_ptr<MissionRender> m_missionDraw;

	// 矢印オブジェクトの描画
	std::unique_ptr<DrawArrow> m_arrowDraw[2];

	// 選択ボックスの描画(次のシーンへ遷移)
	std::unique_ptr<SelectionBox> m_nextSceneBox;

	// ステージ番号描画
	std::unique_ptr<Number> m_stageNumber;

	bool m_changeMachine;							// マシンが変更されたことを知らせるフラグ

};