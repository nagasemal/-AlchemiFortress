#include "pch.h"
#include "AM_Mining.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "Scenes/PlayScene/Field/Crystal/Crystal.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/DataManager.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

#define LVUP_MAGNIFICATION_HP 1.25f
#define STANDARD_HP 100

#define CRYSTAL_SPAN 2.0f

AM_Mining::AM_Mining():
	m_timer()
{
}

AM_Mining::~AM_Mining()
{
}

void AM_Mining::Initialize()
{
	m_machineID = MACHINE_TYPE::MINING;
	m_objectName = "Mining";

	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	m_hp = m_maxHp = ((int)(pSJD.GetMachineData(m_machineID).hp + (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv)));
	m_magicCircle.r = (float)pSJD.GetMachineData(m_machineID).effect_rage;
	m_machineEffectValue = (float)pSJD.GetMachineData(m_machineID).effect_value;

	m_span = CRYSTAL_SPAN;
	
	m_color = Colors::Salmon;
}

void AM_Mining::Update()
{
	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();
	m_maxHp = ((int)(pSJD.GetMachineData(m_machineID).hp + (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv)));


	m_timer += DeltaTime::GetInstance().GetDeltaTime();
	m_color = Colors::Salmon;

	// レベルのリセット
	m_lv = 1;
}

void AM_Mining::SelectUpdate()
{

}

void AM_Mining::Draw()
{
}

void AM_Mining::Finalize()
{
}

int AM_Mining::AllFieldObject(FieldObjectManager* fieldManager)
{

	int crystalPulsVal = 0;

	for (std::list<Crystal>::iterator it = fieldManager->GetCrystalObject()->begin(); it != fieldManager->GetCrystalObject()->end(); it++)
	{
		if (CircleCollider(it->GetCircle(), m_magicCircle))
		{
			//　====================[　色を変更する　]
			m_color = SimpleMath::Color(sinf(m_timer), 0, sinf(m_timer), 1);

			//　====================[　回収開始　]
			//　　|=>　回収する時間がスパンを超えたら
			if (m_timer >= m_span)
			{
				m_timer = 0.0f;

				crystalPulsVal += (int)m_machineEffectValue;
			}
		}
	}

	return crystalPulsVal;
}

void AM_Mining::HitEnemy(std::list<EnemyObject>* enemy)
{

	//　====================[　早期リターン　]
	//　　|=>　無敵時間中
	if (m_invincibleFlag) return;

	for (std::list<EnemyObject>::iterator it = enemy->begin(); it != enemy->end(); it++)
	{

		//　====================[　効果範囲Toエネミー　]
		bool hitMachine = CircleCollider(GetObject3D(), it->GetObject3D());

		if (hitMachine)
		{
			// 体力減少
			m_hp -= (int)it->GetPower();
			// 無敵状態に変更
			m_invincibleFlag = true;

		}
	}
}

bool AM_Mining::GetCrystalFlag()
{
	return m_timer >= m_span;
}

void AM_Mining::RenderUI()
{
}
