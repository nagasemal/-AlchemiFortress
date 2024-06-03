//--------------------------------------------------------------------------------------
// File: SceneObj.h
//
// 
// 
// Date: 2023.4.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "NecromaLib/Singleton/ShareData.h"

// シーンの列挙
enum class GAME_SCENE : int
{
	NONE,

	TITLE,
	PLAY,
	RESULT,
	SELECT,
	EDIT,
};

class SceneObj
{
public:

	virtual ~SceneObj()				= default;

	// 初期化
	virtual void Initialize()		= 0;

	// 更新
	virtual GAME_SCENE Update()		= 0;

	// 描画
	virtual void Draw()				= 0;

	// UI系描画
	virtual void DrawUI()			= 0;

	// 終了処理
	virtual void Finalize()			= 0;

private:

};