//--------------------------------------------------------------------------------------
// File		: TitleScene.h
//
//　タイトルのシステムを制御,実行
// 
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "NecromaLib/SceneObject/SceneObj.h"

#include "Scenes/Commons/DrawMachine.h"
#include "Scenes/TitleScene/Camera/TitleCamera.h"
#include "Scenes/TitleScene/TitleLogo/TitleLogo.h"
#include "Scenes/TitleScene/TitleLogo/Veil.h"

#include "NecromaLib/GameData/Animation.h"
#include "Scenes/Commons/UIKeyControl.h"
#include "Scenes/TitleScene/Option/RenderOption.h"

class MagicCircle;
class DrawLine;
class DrawBox;
class ShaderButton;

class TitleScene : public SceneObj
{
public:
	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene();

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

	enum ButtonType
	{
		NewGame  = 0,
		Restert	 = 1,
		Option	 = 2,
		Exit	 = 3,
		Num
	};

	std::unique_ptr<DrawMachine>		m_titleCall;
	std::unique_ptr<TitleCamera>		m_titleCamera;
	std::unique_ptr<TitleLogo>			m_titleLogo;

	// ボタン
	std::unique_ptr<DrawLine>			m_selectionButton[ButtonType::Num];

	std::unique_ptr<MagicCircle>		m_magicCircle;

	std::unique_ptr<DirectX::Model>		m_skySphere;

	std::unique_ptr<Veil>				m_veil;

	std::unique_ptr<UIKeyControl>		m_uiKeyControl;

	std::unique_ptr<RenderOption>		m_renderOption;

	AnimationData m_animationData;
	bool m_rotateNowFlag;

	// 押したボタンの状態を保存しておく
	ButtonType m_nextType;

	float m_rotateYSpeed;

};