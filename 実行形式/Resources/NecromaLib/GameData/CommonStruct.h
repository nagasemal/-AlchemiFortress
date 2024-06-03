//--------------------------------------------------------------------------------------
// File: CommonStruct.h
//
//　よく使う構造体,列挙型をまとめたクラス
// 
// Author: KazumaNagase
//--------------------------------------------------------------------------------------

#pragma once

enum ELEMENT : int
{
	NOMAL = 0,
	FLAME = 1,
	AQUA = 2,
	WIND = 3,
	EARTH = 4,
	Num
};

// ミッションの種類
enum MISSION_TYPE : int
{
	SPAWN,		// マシンの生成
	ALCHEMI,	// マシンの錬金
	ENEMY_KILL,		// エネミーの討伐
	BASELV,		// 拠点レベル
	RESOURCE,	// リソース特定量取得
	TIMER,		// 時間

	MISSION_NUM

};

// キーを列挙型として持っておけるようにする
enum MACHINE_TYPE : int
{
	NONE		= 0,	// 指定なし
	ATTACKER	= 1,	// 攻撃型
	DEFENSER	= 2,	// 防御型
	UPPER		= 3,	// 範囲内強化型
	RECOVERY	= 4,	// 魔力回収型
	MINING		= 5,	// 採掘型

	NUM
};

enum ENEMY_TYPE : int
{

	ENMEY_NONE = 0,

	SLIME	= 1,	// 通常
	WORM	= 2,	// 蛇行
	HOP		= 3,	// ジャンプしながら
	RETREAT = 4,	// 行って戻る

	ENEMY_NUM
};


namespace ChangeData
{
	// string形式をMACHINE_ELEMENTに変換
	static ELEMENT ChangeElement(std::string element)
	{
		ELEMENT elements = ELEMENT::NOMAL;

		if (element == "Nomal") elements = ELEMENT::NOMAL;
		else if (element == "Flame") elements = ELEMENT::FLAME;
		else if (element == "Aqua")	 elements = ELEMENT::AQUA;
		else if (element == "Wind")	 elements = ELEMENT::WIND;
		else if (element == "Earth") elements = ELEMENT::EARTH;

		return elements;
	}

	static std::string ChangeElementString(const ELEMENT element)
	{
		switch (element)
		{
		case NOMAL:
			return "Nomal";
		case FLAME:
			return "Flame";
		case AQUA:
			return "Aqua";
		case WIND:
			return "Wind";
		case EARTH:
			return "Earth";
		default:
			return "Nomal";
		}

	}

	// string形式をMACHINE_TYPEに変換
	static MACHINE_TYPE ChangeMachine(const std::string machine)
	{
		MACHINE_TYPE type = MACHINE_TYPE::NONE;

		if (machine == "None")		type = MACHINE_TYPE::NONE;
		else if (machine == "Attacker")	type = MACHINE_TYPE::ATTACKER;
		else if (machine == "Upper")	type = MACHINE_TYPE::UPPER;
		else if (machine == "Defenser")	type = MACHINE_TYPE::DEFENSER;
		else if (machine == "Mining")	type = MACHINE_TYPE::MINING;
		else if (machine == "Recovery")	type = MACHINE_TYPE::RECOVERY;

		return type;
	}

	// MACHINE_TYPEをstring形式に変換
	static std::string ChangeMachineString(const MACHINE_TYPE type)
	{
		switch (type)
		{
		case NONE:
			return "None";
		case ATTACKER:
			return "Attacker";
		case UPPER:
			return "Upper";
		case DEFENSER:
			return "Defenser";
		case MINING:
			return "Mining";
		case RECOVERY:
			return "Recovery";
		default:
			return "None";
		}

	}

	// string形式をENEMY_TYPEに変換
	static ENEMY_TYPE ChangeEnemy(const std::string enemy_Name)
	{
		ENEMY_TYPE type = ENEMY_TYPE::ENMEY_NONE;

		if (enemy_Name == "None")			type = ENEMY_TYPE::ENMEY_NONE;
		else if (enemy_Name == "Slime")		type = ENEMY_TYPE::SLIME;
		else if (enemy_Name == "Worm")		type = ENEMY_TYPE::WORM;
		else if (enemy_Name == "Hop")		type = ENEMY_TYPE::HOP;
		else if (enemy_Name == "Retreat")	type = ENEMY_TYPE::RETREAT;

		return type;
	}

	// ENEMY_TYPEをstring形式に変換
	static std::string ChangeEnemyString(const ENEMY_TYPE enemy_Type)
	{

		switch (enemy_Type)
		{
		case ENMEY_NONE:
			return "None";
		case SLIME:
			return "Slime";
		case WORM:
			return "Worm";
		case HOP:
			return "Hop";
		case RETREAT:
			return "Retreat";
		default:
			break;
		}

		return "None";
	}

	// ELEMENTをColorに変換
	static SimpleMath::Color ChangeColor(ELEMENT element)
	{
		SimpleMath::Color colors = SimpleMath::Color(0.8f,0.8f,0.8f,1.0f);

		switch (element)
		{
		case NOMAL:
			return SimpleMath::Color(0.8f, 0.8f, 0.8f, 1.0f);
		case FLAME:
			return SimpleMath::Color(1.0f, 0.2f, 0.2f, 1.0f);
		case AQUA:
			return SimpleMath::Color(0.2f, 0.2f, 1.0f, 1.0f);
		case WIND:
			return SimpleMath::Color(0.2f, 1.0f, 0.2f, 1.0f);
		case EARTH:
			return SimpleMath::Color(0.8f, 0.8f, 0.2f, 1.0f);
		default:
			break;
		}

		return colors;
	}
}