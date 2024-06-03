#include <pch.h>
#include "DataManager.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

DataManager* DataManager::instance = nullptr;

DataManager::DataManager() :
	m_nowMP			(),
	m_nowCrystal	(),
	m_nowBaseHp		(),
	m_nowCrystal_MAX(),
	m_nowMP_MAX		(),
	m_nowBaseHp_MAX	(),
	m_round			(),
	m_nowEnemyKill	(),
	m_stageNum		(1),
	m_stageClearFlag()
{

}

void DataManager::Initialize()
{
	auto resource = ShareJsonData::GetInstance().GetStageData().resource;

	MPMAXRecalculation(resource.lv);
	m_nowMP				= resource.mp;

	CrystalMAXRecalculation(resource.lv);
	m_nowCrystal		= resource.crystal;

	BaseHPMAXRecalculation(resource.lv);
	m_nowBaseHp			= resource.hp;

	m_nowEnemyKill = 0;
	m_round = 1;
}

void DataManager::Update()
{
	// è„å¿â∫å¿ê›íË
	m_nowMP = std::min(std::max(m_nowMP, 0), m_nowMP_MAX);

	m_nowCrystal = std::min(std::max(m_nowCrystal, 0), m_nowCrystal_MAX);

	m_nowBaseHp = std::min(std::max(m_nowBaseHp, 0), m_nowBaseHp_MAX);

}

void DataManager::MPMAXRecalculation(int lv)
{
	auto gameParam = ShareJsonData::GetInstance().GetGameParameter();

	m_nowMP_MAX = gameParam.mp_Max * lv / gameParam.baseLV_MAX;
}

void DataManager::CrystalMAXRecalculation(int lv)
{
	auto gameParam = ShareJsonData::GetInstance().GetGameParameter();

	m_nowCrystal_MAX = gameParam.crystal_Max * lv / gameParam.baseLV_MAX;
}

void DataManager::BaseHPMAXRecalculation(int lv)
{
	auto gameParam = ShareJsonData::GetInstance().GetGameParameter();

	m_nowBaseHp_MAX = gameParam.baseHp_Max * lv / gameParam.baseLV_MAX;

	// ëSâÒïú
	m_nowBaseHp = m_nowBaseHp_MAX;

}


//void DataManager::Create()
//{
//	if (instance == nullptr)
//	{
//		instance = new DataManager;
//	}
//}
//
//void DataManager::Destroy()
//{
//	if (instance != nullptr)
//	{
//		delete instance;
//		instance = nullptr;
//	}
//}