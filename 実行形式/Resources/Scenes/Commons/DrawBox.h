//--------------------------------------------------------------------------------------
// File: DrawBox.h
//
//　枠線の描画
// 
// Date: 2024.11.6
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "Scenes/Commons/SelectionUI.h"
#include "NecromaLib/GameData/Animation.h"

class DrawClick;
class Particle_2D;

class DrawBox : public SelectionUI
{
public:
	DrawBox(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage,float lineRage = 2.5f);
	~DrawBox();

	// 更新処理
	void Update()				override;

	// 描画
	void Draw()					override;

	void ResetAnimationData() { m_animationData_Second.time = m_animationData_First.time = 0.0f; }
	void SetColor(SimpleMath::Color color) { m_color = color; }

	void SetAnimationFlag(bool flag) { m_animationFlag = flag; }

	void SetPosRage(SimpleMath::Vector2 pos,SimpleMath::Vector2 rage);

private:

	AnimationData m_animationData_First;
	AnimationData m_animationData_Second;

	std::unique_ptr<DrawClick> m_clickPointer;

	float m_rotate_easing;

	SimpleMath::Vector2 m_lineRage_Vertical;
	SimpleMath::Vector2 m_lineRage_Beside;
	float m_rotate;

	// 動く演出用のポジション
	SimpleMath::Vector2 m_boxPos;

	SimpleMath::Color m_color;

	// 触れている判定を無視して強制的にアニメーションをさせるフラグ
	bool m_animationFlag;

};