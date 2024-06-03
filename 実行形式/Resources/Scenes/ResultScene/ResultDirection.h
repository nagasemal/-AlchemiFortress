//--------------------------------------------------------------------------------------
// File: ResultDirection.h
//
//　リザルトシーンにおける魔法陣描画
// 
// Date: 2024.1.18
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "NecromaLib/GameData/Animation.h"

class ResultDirection
{
public:
	ResultDirection();
	~ResultDirection();

	void Update();

	void Render();

private:

	AnimationData m_animationData;

	bool m_rotateNowFlag;

};