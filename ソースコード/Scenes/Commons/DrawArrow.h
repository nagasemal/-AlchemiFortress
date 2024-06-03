//--------------------------------------------------------------------------------------
// File: DrawArrow.h
//
//Å@ñÓàÛÇÃUIï\é¶
//  
// 
// Date: 2023.8.25
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "Scenes/Commons/SelectionUI.h"

class DrawArrow : public SelectionUI
{
public:

	DrawArrow(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage,int direciton);
	~DrawArrow();

	void Update()				override;
	// ï`âÊ
	void Draw()					override;

	void SetDirection(int direction) { m_direction = direction; }

	DrawKey* GetDrawKey() { return m_drawKey.get(); }

private:

	int m_direction;

	std::unique_ptr<DrawKey> m_drawKey;

};