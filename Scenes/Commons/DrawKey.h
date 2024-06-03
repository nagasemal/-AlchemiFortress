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