#include "pch.h"
#include "AM_Defenser.h"
#include "Scenes/PlayScene/Enemy/EnemyManager.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

#include "NecromaLib/Singleton/DeltaTime.h"

AM_Defenser::AM_Defenser():
	m_isBreak(),
	m_counterFlag()
{
}

AM_Defenser::~AM_Defenser()
{
}

void AM_Defenser::Initialize()
{
	m_machineID = MACHINE_TYPE::DEFENSER;
	m_objectName = "Defenser";

	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();
	m_hp = m_maxHp = ((int)(pSJD.GetMachineData(m_machineID).hp + (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv)));

	m_color = Colors::DarkSlateBlue;

}

void AM_Defenser::Update()
{
	m_data.pos.y = 0.0f;

	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();
	m_maxHp = ((int)(pSJD.GetMachineData(m_machineID).hp + (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv)));

	// レベルのリセット
	m_lv = 1;
}

void AM_Defenser::SelectUpdate()
{

}

void AM_Defenser::Draw()
{
}

void AM_Defenser::Finalize()
{
}

void AM_Defenser::RenderUI()
{

}

void AM_Defenser::EnemyHit(std::list<EnemyObject>* enemys)
{

	m_counterFlag = false;

	//　現存存在するエネミー分回す
	//	効果範囲toエネミー
	for (std::list<EnemyObject>::iterator it = enemys->begin(); it != enemys->end(); it++)
	{
		// ダウンキャストを行い、GameObject3D型に変換し判定の処理を得る
		bool hitMachine = CircleCollider(GetObject3D(),it->GetObject3D());

		if (hitMachine)
		{
			it->HitMachine(hitMachine);
			it->Bouns();

			if (m_invincibleFlag) continue;

			// 体力減少
			m_hp -= (int)it->GetPower();
			it->HitBullet(it->GetPos() - m_data.pos, m_lv);
			m_invincibleFlag = true;
			m_counterFlag = true;
		}
	}
}

bool AM_Defenser::CounterAttack()
{
	return m_counterFlag;
}
