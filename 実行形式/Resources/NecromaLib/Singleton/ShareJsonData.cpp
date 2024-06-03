#include "pch.h"
#include "ShareJsonData.h"
#include "NecromaLib/GameData/Easing.h"

ShareJsonData* ShareJsonData::instance = nullptr;

ShareJsonData::ShareJsonData():
	m_bulletData{}
{

}

void ShareJsonData::Create()
{
	if (instance == nullptr)
	{
		instance = new ShareJsonData;
	}
}

void ShareJsonData::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void ShareJsonData::LoadingJsonFile_Bullet()
{

	m_bulletData[ELEMENT::NOMAL]	= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Nomal.json");
	m_bulletData[ELEMENT::FLAME]	= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Flame.json");
	m_bulletData[ELEMENT::AQUA]		= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Aqua.json");
	m_bulletData[ELEMENT::WIND]		= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Wind.json");
	m_bulletData[ELEMENT::EARTH]	= Json::FileLoad_BulletData("Resources/Json/BulletData/BulletData_Earth.json");

}

void ShareJsonData::LoadingJsonFile_Machine()
{
	m_machineData[MACHINE_TYPE::NONE]		= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_None.json");
	m_machineData[MACHINE_TYPE::ATTACKER]	= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_Attacker.json");
	m_machineData[MACHINE_TYPE::DEFENSER]	= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_Defenser.json");
	m_machineData[MACHINE_TYPE::MINING]		= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_Mining.json");
	m_machineData[MACHINE_TYPE::RECOVERY]	= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_Recovery.json");
	m_machineData[MACHINE_TYPE::UPPER]		= Json::FileLoad_MachineData("Resources/Json/MachineData/MachineData_Upper.json");
}

void ShareJsonData::LoadingJsonFile_Enemy()
{
	m_enemyData[ENEMY_TYPE::ENMEY_NONE] = Json::FileLoad_EnemyData("Resources/Json/EnemyData/EnemyData_None.json");
	m_enemyData[ENEMY_TYPE::SLIME] = Json::FileLoad_EnemyData("Resources/Json/EnemyData/EnemyData_Slime.json");
	m_enemyData[ENEMY_TYPE::WORM] = Json::FileLoad_EnemyData("Resources/Json/EnemyData/EnemyData_Worm.json");
	m_enemyData[ENEMY_TYPE::HOP] = Json::FileLoad_EnemyData("Resources/Json/EnemyData/EnemyData_Hop.json");
	m_enemyData[ENEMY_TYPE::RETREAT] = Json::FileLoad_EnemyData("Resources/Json/EnemyData/EnemyData_Retreat.json");
}

void ShareJsonData::LoadingJsonFile_GameParameter()
{

	m_gameParam = Json::FileLoad_GameParameter("Resources/Json/GameParameter/GameParameter.json");

}

void ShareJsonData::LoadingJsonFile_Stage(int number, int wave)
{

	// numberに応じたファイルパスを読み込む
	std::ostringstream oss;
	oss << "Stage_" << number << "/StageData_" << number << "_" << wave;

	std::string filePath = "Resources/Json/StageData/" + oss.str() + ".json";

	//　====================[　引き継ぐ情報を保存する　]
	//　　|=>　クリスタルの位置
	std::vector<SimpleMath::Vector2> saveCrystalPos;
	if (wave > 1)	saveCrystalPos = m_stageData.crystalPos;

	//　====================[　ステージデータを読み込む　]
	m_stageData = Json::FileLoad_StageData(filePath);

	//　====================[　保存した情報を再度入れなおす　]
	if (wave > 1)	m_stageData.crystalPos = saveCrystalPos;
}

void ShareJsonData::LoadingJsonFile_ClearData(int number)
{
	m_clearData = Stage_ClearData();

	// numberに応じたファイルパスを読み込む
	std::ostringstream oss;
	oss << number;
	std::string filePath = "Resources/Json/ClearData/ClearData_" + oss.str() + ".json";

	m_clearData = Json::FileLoad_StageClearData(filePath);
}

void ShareJsonData::LoadingUIData(std::string pathName,std::string tagName)
{
	std::string tagPath = pathName + tagName;

	//　====================[　新しく連想配列を生成　]
	m_uiDatas.insert(std::make_pair(tagPath,UI_Data()));

	//　====================[　tagNameに応じたファイルパスを生成　]
	std::ostringstream ossPath;
	ossPath << pathName;

	std::ostringstream ossName;
	ossName << tagName;

	std::string filePath = "Resources/Json/GameParameter/" + ossPath.str() + "/UILayout_" + ossPath.str() + ossName.str() + ".json";



	//　====================[　生成したデータを代入　]
	m_uiDatas[tagPath] = Json::FileLoad_UIData(filePath);

	//　====================[　Offsetが名前に入っている時の処理　]
	//　　|=>　[変数]　タグを識別する
	std::string tag = std::string();

	for (size_t i = 0; i < tagPath.size(); i++)
	{
		//　　|=>　[条件]　入力されたタグのi番目以降の配列がOffset
		if (tagPath.substr(i) == "Offset")
		{
			//　　|=>　Offset前のタグ名を取得する
			tag = tagPath.substr(0,i);
		}

	}

	// タグのサイズが0ならば処理を行う必要はない
	if (tag.size() == 0) return;

	for (auto it = m_uiDatas.begin(); it != m_uiDatas.end(); it++)
	{
		//　　|=>　抜き出したタグ名と同値のタグ名を検出
		if (it->first.substr(0, tag.size()) == tag)
		{
			if (it->first.substr(tag.size()) == "Offset") continue;

			//　　|=>　位置,大きさの情報を加算する
			it->second.pos	+= m_uiDatas[tagPath].pos;
			it->second.rage	+= m_uiDatas[tagPath].rage;
		}
	}


}

void ShareJsonData::LoadingParticle2D(std::string pathName)
{

	//　====================[　tagNameに応じたファイルパスを生成　]
	std::ostringstream ossPath;
	ossPath << pathName;

	std::string filePath = "Resources/Json/Particle2D/" + ossPath.str() + ".json";

	//　====================[　生成したデータを代入　]
	Particle2D_Pram parm = Json::FileLoad_Particle2D(filePath);

	//　====================[　新しく連想配列を生成　]
	m_particlePrams.insert(std::make_pair(parm.tag, parm));
}

void ShareJsonData::SetEndlessStageData(Stage_Data data)
{

	m_stageData = data;

}

const Bullet_Data ShareJsonData::GetBulletData(ELEMENT element)
{
	return m_bulletData[element];
}

const Machine_Data ShareJsonData::GetMachineData(MACHINE_TYPE type)
{
	return m_machineData[type];
}

const Enemy_Data ShareJsonData::GetEnemyData(ENEMY_TYPE type)
{
	return m_enemyData[type];
}

const Stage_Data ShareJsonData::GetStageData()
{
	return m_stageData;
}

const Stage_ClearData ShareJsonData::GetClearData()
{
	return m_clearData;
}

const Game_Parameter ShareJsonData::GetGameParameter()
{
	return m_gameParam;
}

const UI_Data ShareJsonData::GetUIData(std::string tagName)
{

	if (&m_uiDatas[tagName] == nullptr)
	{
		assert("存在していないUIデータを参照しています");
	}

	return m_uiDatas[tagName];
}

const Particle2D_Pram ShareJsonData::GetParticle2DPram(std::string tagName)
{
	return m_particlePrams[tagName];
}

void ShareJsonData::StageDataCleanUP()
{

	m_stageData = Stage_Data();

}

SimpleMath::Color ShareJsonData::GetElementColor(ELEMENT element)
{
	return ChangeData::ChangeColor(element);
}
