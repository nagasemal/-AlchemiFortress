#pragma once

class SelectionUI;

// コマンドパターン　：　ICommand_UI

struct UI_Data
{
	SelectionUI* ui;	// 対象となるUIのポインター
	int row;	// 列
	int col;	// 行

};

// キーボードでUI操作を完結させられるようにする処理に使用
class ICommand_UI
{
public:

	virtual UI_Data GetUIData() = 0;

	virtual void SetUIData(UI_Data ui) = 0;

	// 処理実行関数
	virtual void Execute() = 0;

	virtual ~ICommand_UI() = default;

};