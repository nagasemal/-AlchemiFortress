//--------------------------------------------------------------------------------------
// File: DrawKey.h
//
//　アルファベットキーを描画
// 
// Date: 2024.11.6
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "Scenes/Commons/SelectionUI.h"

class DrawKey
{
public:
	DrawKey(SelectionUI* parent);
	~DrawKey();

	void Update();
	void Draw();

	bool PushKey();

	// アルファベットのキーを返します
	void DrawAlphabetKey(int cutVal);

private:

	SelectionUI* m_parent;

};