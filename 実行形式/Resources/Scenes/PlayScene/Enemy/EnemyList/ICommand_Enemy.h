//--------------------------------------------------------------------------------------
// File: ICommand＿Enemy.h
//
//　エネミーコマンドのインターフェース
// 
// Date: 2023.10.26
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class EnemyObject;

struct MoveParameter
{
	float delay = 0;		// コマンド移行する際に止める場合は記述
	float time	= 0;		// 稼働時間
	float value = 0;		// 行動に置ける強さ
};

class ICommand_Enemy
{
public:
	virtual MoveParameter GetParam() = 0;

	virtual bool GetCompletion() = 0;

	virtual void SetCompletion(bool flag) = 0;

	virtual void SetParam(MoveParameter param) = 0;

	virtual void SetEnemyPtr(EnemyObject& object) = 0;

	virtual void Reset() = 0;

	// 処理実行関数
	virtual void Execute() = 0;

	virtual ~ICommand_Enemy() = default;

};