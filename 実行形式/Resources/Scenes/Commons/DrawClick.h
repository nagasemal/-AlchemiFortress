//--------------------------------------------------------------------------------------
// File: DrawClick.h
//
//　クリックするべき場所の示唆を描画
// 
// Date: 2024.11.6
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------


#pragma once

#include "NecromaLib/GameData/GameObject2D.h"
#include "NecromaLib/GameData/Animation.h"

class DrawClick : public GameObject2D
{
public:
	DrawClick(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage);
	~DrawClick();

	void Initialize()			override;
	// 更新処理
	void Update()				override;
	// 描画
	void Draw()					override;

	void Finalize()				override;

private:

	AnimationData m_animationData;

};