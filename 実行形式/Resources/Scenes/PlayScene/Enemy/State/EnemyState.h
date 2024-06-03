//--------------------------------------------------------------------------------------
// File: EnemyState.h
//
//　エネミーの状態に関する処理が書かれたファイル
// 
// Date: 2023.10.26
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "IEnemyState.h"

class MagicCircle;

// エネミーがスポーンする前の状態
// 魔法陣が出現する
class Enemy_IdlingState : public IEnemyState
{
public:
	Enemy_IdlingState();
	~Enemy_IdlingState() = default;

	void Update(EnemyObject* object)				override;
	void Render(EnemyObject* object,Model* model)	override;
	void RenderIcon(EnemyObject* object)			override;

private:

	std::unique_ptr<MagicCircle> m_magicCircle;

};


// エネミーが動く状態
class Enemy_MoveingState : public IEnemyState
{
public:
	Enemy_MoveingState();
	~Enemy_MoveingState() = default;

	void Update(EnemyObject* object) override;
	void Render(EnemyObject* object, Model* model) override;
	void RenderIcon(EnemyObject* object)			override;

private:

};

// エネミーが止まる状態
class Enemy_StoppingState : public IEnemyState
{
public:
	Enemy_StoppingState();
	~Enemy_StoppingState() = default;

	void Update(EnemyObject* object) override;
	void Render(EnemyObject* object,Model* model) override;
	void RenderIcon(EnemyObject* object)			override;

private:

};

// エネミーがノックバックしている状態(無敵状態 / 攻撃は行えない)
class Enemy_KnockBackState : public IEnemyState
{
public:

	/// <summary>
	/// エネミーがdirection方向に吹き飛ばされます
	/// </summary>
	/// <param name="direction">吹き飛ばす方向ベクトル</param>
	/// <param name="power">吹き飛ばす力</param>
	Enemy_KnockBackState(SimpleMath::Vector3 direction,float power);
	~Enemy_KnockBackState() = default;

	void Update(EnemyObject* object) override;
	void Render(EnemyObject* object,Model* model) override;
	void RenderIcon(EnemyObject* object)			override;

private:

	SimpleMath::Vector3 m_knockBackVec;

};

// エネミーが死んでいる状態
class Enemy_DethState : public IEnemyState
{
public:

	Enemy_DethState();
	~Enemy_DethState() = default;

	void Update(EnemyObject* object) override;
	void Render(EnemyObject* object,Model* model) override;
	void RenderIcon(EnemyObject* object)			override;

private:

	const float DETH_TIME = 5.0f;

private:

	float m_dethTime;

};