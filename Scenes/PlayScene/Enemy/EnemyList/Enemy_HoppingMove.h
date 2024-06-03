#pragma once

#include "ICommand_Enemy.h"

class Enemy_HoppingMove : public ICommand_Enemy
{
public:
	Enemy_HoppingMove();
	~Enemy_HoppingMove();

	// 実行
	void Execute()override;

	MoveParameter GetParam() override;

	bool GetCompletion() override;

	void SetCompletion(bool flag) override;

	void SetParam(MoveParameter param) override;

	void SetEnemyPtr(EnemyObject& object) override;

	void Reset() override;

private:

	MoveParameter m_param;	// コマンドのパラメータ
	EnemyObject* m_enemyPtr;	// 対象のポインター

	float m_time;			// コマンドが開始してから経過した時間
	bool m_completion;		// コマンドが終了したことを知らせるフラグ

	float m_hoppingVal;

};