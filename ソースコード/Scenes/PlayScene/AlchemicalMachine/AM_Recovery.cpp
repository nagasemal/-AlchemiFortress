#include "pch.h"
#include "AM_Recovery.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "NecromaLib/Singleton/ShareJsonData.h"

AM_Recovery::AM_Recovery()
{
}

AM_Recovery::~AM_Recovery()
{
}

void AM_Recovery::Initialize()
{

	m_machineID = MACHINE_TYPE::RECOVERY;
	m_objectName = "Recovery";

	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();
	m_hp = m_maxHp = ((int)(pSJD.GetMachineData(m_machineID).hp + (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv)));
	m_machineEffectValue = pSJD.GetMachineData(m_machineID).effect_value;

	m_color = Colors::DarkCyan;
}

void AM_Recovery::Update()
{
	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();
	m_maxHp = ((int)(pSJD.GetMachineData(m_machineID).hp + (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv)));


	m_magicCircle.p = m_data.pos;

	// レベルのリセット
	m_lv = 1;

}

void AM_Recovery::SelectUpdate()
{

}

void AM_Recovery::Draw()
{
}

void AM_Recovery::Finalize()
{
}

void AM_Recovery::RenderUI()
{
}

void AM_Recovery::HitEnemy(std::list<EnemyObject>* enemy)
{

	// TRUEならば早急に弾く
	if (m_invincibleFlag) return;

	//　現存存在するエネミー分回す
	//	効果範囲toエネミー
	for (std::list<EnemyObject>::iterator it = enemy->begin(); it != enemy->end(); it++)
	{
		// ダウンキャストを行い、GameObject3D型に変換し判定の処理を得る
		bool hitMachine = CircleCollider(GetObject3D(), it->GetObject3D());

		if (hitMachine)
		{
			// 体力減少
			m_hp -= (int)it->GetPower();
			m_invincibleFlag = true;

		}
	}
}

void AM_Recovery::MPPuls(DataManager* pDM)
{
	// MPを増やす
	pDM->SetNowMP(pDM->GetNowMP() + (int)(m_lv * m_machineEffectValue));

}
