//--------------------------------------------------------------------------------------
// File		: ResultScene.h
//
//　リザルトのシステムを制御,実行
// 
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

class SelectionBox;
class UIKeyControl;
class Veil;
class DrawMachine;
class ResultCamera;
class ResultDirection;

class ResultScene : public SceneObj
{
public:
	// コンストラクタ
	ResultScene();

	// デストラクタ
	~ResultScene();

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

private:

	std::unique_ptr<SelectionBox> m_selectionBox_Next;
	std::unique_ptr<SelectionBox> m_selectionBox_Retry;
	std::unique_ptr<SelectionBox> m_selectionBox_Back;

	std::unique_ptr<DrawMachine> m_drawMachine;

	std::unique_ptr<Veil>		  m_veil;

	std::unique_ptr<UIKeyControl> m_uiKeyControl;

	std::unique_ptr<ResultCamera> m_resultCamera;

	std::unique_ptr<ResultDirection> m_resultDirection;

	std::unique_ptr<DirectX::Model> m_baseModel;

	std::unique_ptr<DirectX::Model>	m_skySphere;

};