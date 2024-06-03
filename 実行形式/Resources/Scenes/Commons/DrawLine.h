//--------------------------------------------------------------------------------------
// File: DrawLine.h
//
//Å@ê¸ÇÃï`âÊ
// 
// Date: 2024.11.6
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "Scenes/Commons/SelectionUI.h"
#include "NecromaLib/GameData/Animation.h"

class DrawLine : public SelectionUI
{
public:
	DrawLine(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage, float lineRage = 2.5f);
	~DrawLine();

	void Update()				override;
	// ï`âÊ
	void Draw()					override;

	void ResetAnimationData() { m_animationData.time = 0.0f;}
	void SetColor(SimpleMath::Color color) { m_color = color; }

private:

	AnimationData m_animationData;

	SimpleMath::Vector2 m_popPos;
	SimpleMath::Vector2 m_lineRage;
	SimpleMath::Vector2 m_ancerRect;

	SimpleMath::Color m_color;

};