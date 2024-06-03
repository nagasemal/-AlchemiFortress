//--------------------------------------------------------------------------------------
// File: UIKeyControl.h
//
// キーボード上でUIを操作する処理
// 
// Date: 2023.8.29
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include <vector>

class SelectionUI;

class UIKeyControl
{
public:

	struct UI_Data
	{
		SelectionUI* ui;
		int low; // 行
		int col; // 列
	};

public:
	UIKeyControl();
	~UIKeyControl();

	void Update(SimpleMath::Vector2 moveNum = SimpleMath::Vector2(100.0f,100.0f));

	void AddUI(SelectionUI* ui);

	void DeleteUI(SelectionUI* ui);

private:
	// 最も近い
	void LookUpPosition();

	bool ArrowKeyPush();
private:

	std::vector<SelectionUI*> m_selectionUIs;

	SimpleMath::Vector2 m_nowUIPos;

	int m_index;
	int m_prevIndex;

private:

	std::vector<int> m_maxLows;

	SimpleMath::Vector2 m_prevMousePos; // 前のマウスのポジションの取得
	bool m_keyContorlFlag;

};