#include "pch.h"
#include "Enemy_StandardMove.h"

#include "Scenes/PlayScene/Enemy/EnemyObject.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/Singleton/DeltaTime.h"

Enemy_StanderdMove::Enemy_StanderdMove() :
	m_completion(),
	m_time(),
	m_enemyPtr(nullptr),
	m_param()
{
}

Enemy_StanderdMove::~Enemy_StanderdMove()
{
}

void Enemy_StanderdMove::Execute()
{
	m_time += DeltaTime::GetInstance().GetDeltaTime();

	// 待機時間が来るまでは処理を行わない
	if (m_time <= m_param.delay || m_enemyPtr->GetStopFlag()) return;

	// 速度の計算
	SimpleMath::Vector3 accele = SimpleMath::Vector3(m_param.value,0.0f, m_param.value);

	m_enemyPtr->SetAccele(m_enemyPtr->GetAccele() + accele);

	// 移動完了
	m_completion = m_time >= m_param.time;
}

MoveParameter Enemy_StanderdMove::GetParam()
{
	return m_param;
}

bool Enemy_StanderdMove::GetCompletion()
{
	return m_completion;
}

void Enemy_StanderdMove::SetCompletion(bool flag)
{
	m_completion = flag;
}

void Enemy_StanderdMove::SetParam(MoveParameter param)
{
	m_param.delay = param.delay;
	m_param.time = param.time;
	m_param.value = param.value;
}

void Enemy_StanderdMove::SetEnemyPtr(EnemyObject& object)
{
	m_enemyPtr = &object;
}

void Enemy_StanderdMove::Reset()
{
	m_time = 0.0f;
}
