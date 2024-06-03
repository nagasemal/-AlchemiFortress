#include "pch.h"
#include "AM_Attacker.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/SoundData.h"

#include "NecromaLib/GameData/SpriteCutter.h"

AM_Attacker::AM_Attacker():
	m_timer(),
	m_targetPos(),
	m_saveBulletStatus(),
	m_bulletStatus()
{
}

AM_Attacker::~AM_Attacker()
{
}

void AM_Attacker::Initialize()
{
	m_machineID = MACHINE_TYPE::ATTACKER;
	m_objectName = "Attacker";

	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();
	m_hp = m_maxHp = ((int)(pSJD.GetMachineData(m_machineID).hp * (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv)));

	m_color = Colors::LightSlateGray;

	// 1Lv時のバレットのステータスを受け取る
	m_bulletStatus = RecalculationStatus(m_element,m_lv);

	// 保存
	m_saveBulletStatus = m_bulletStatus;
}

void AM_Attacker::Update()
{
	// Jsonから読み取ったマシンのデータを適応する
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	//　====================[　最大HP強化　]
	m_maxHp = ((int)(pSJD.GetMachineData(m_machineID).hp + (pSJD.GetMachineData(m_machineID).multiplier_hp * m_lv)));

	// 現在レベルで再計算
	m_bulletStatus = RecalculationStatus(m_element, m_lv);

	// 値リセット
	m_powerUPFlag = false;
	m_lv = 1;

}

void AM_Attacker::SelectUpdate()
{
}

void AM_Attacker::Draw()
{
}

void AM_Attacker::Finalize()
{
}

bool AM_Attacker::BulletRequest(std::list<EnemyObject>* enemys)
{

	DataManager* pDataM = DataManager::GetInstance();
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_timer += deltaTime;

	//　現存存在するエネミー分回す
	//	効果範囲toエネミー
	for (std::list<EnemyObject>::iterator it = enemys->begin(); it != enemys->end(); it++)
	{

		// ダウンキャストを行い、GameObject3D型に変換し判定の処理を得る
		bool hitMachine = CircleCollider(GetObject3D(), it->GetObject3D());

		// 自機とエネミーの当たり判定
		if (hitMachine && !m_invincibleFlag)
		{
			// ダメージを受ける
			m_hp -= (int)it->GetPower();

			m_invincibleFlag = true;
		}

		if (CircleCollider(it->GetCircle(), m_magicCircle) && it->GetColliderActive())
		{
			// スパン毎に生成
			if (m_timer >= m_bulletStatus.span && pDataM->GetNowMP() > 0)
			{
				SoundData& pSound = SoundData::GetInstance();

				pSound.PlaySE(ConvertToElement(m_element));

				m_timer = 0.0f;
				m_targetPos = it->GetPos();

				// 消費MPはLvに依存(高いほど消費大)
				pDataM->SetNowMP(pDataM->GetNowMP() - (int)(m_bulletStatus.lossMp * m_lv));

				return true;
			}
		}
	}

	return false;

}

Bullet AM_Attacker::GetBulletData()
{
	Bullet::BulletData data = {};
	data.damage	 = (float)m_bulletStatus.str;
	data.life	 = (float)m_bulletStatus.life;
	data.speed	 = (float)m_bulletStatus.speed;

	return Bullet(data, m_color, m_data.pos,m_targetPos);
}

void AM_Attacker::RenderUI()
{
}