#include "pch.h"
#include "AM_Upper.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#define HEALING_TIME 5.0f

AM_Upper::AM_Upper():
	m_healingTime()
{
}

AM_Upper::~AM_Upper()
{
}

void AM_Upper::Initialize()
{
	m_machineID = MACHINE_TYPE::UPPER;
	m_objectName = "Upper";

	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();
	m_hp = m_maxHp = ((int)(pSJD.GetMachineData(m_machineID).hp * (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv)));

	// 効果範囲をLvに応じて決める
	m_magicCircle.r = pSJD.GetMachineData(m_machineID).effect_rage + (pSJD.GetMachineData(m_machineID).multiplier_effect * m_lv);

	m_color = Colors::YellowGreen;

}

void AM_Upper::Update()
{

	// 回復開始までの時間
	m_healingTime += DeltaTime::GetInstance().GetDeltaTime();

	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();
	m_maxHp = ((int)(pSJD.GetMachineData(m_machineID).hp + (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv)));

	// レベルのリセット
	m_lv = 1;


}

void AM_Upper::SelectUpdate()
{
}

void AM_Upper::Draw()
{
}

void AM_Upper::Finalize()
{

}

void AM_Upper::RenderUI()
{

}

void AM_Upper::HitEnemy(std::list<EnemyObject>* enemy)
{

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

void AM_Upper::MachinePowerUP(std::vector<std::shared_ptr<AlchemicalMachineObject>> objects)
{
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	for (auto& object : objects)
	{

		if (object.get() == this) continue;

		if (CircleCollider(GetMagicCircle(), object->GetCircle()))
		{
			object->SetLv(object->GetLv() + 1);
			object->SetPowerUpFlag(true);

			// 回復開始
			if (m_healingTime >= HEALING_TIME && object->GetHP() > 0.0f)
			{
				object->SetHP(object->GetHP() + (int)pSJD.GetMachineData(m_machineID).effect_value * m_lv);
			}
		}
	}

	// 回復値リセット
	if (m_healingTime >= HEALING_TIME)
	{
		m_healingTime = 0.0f;
	}

}
