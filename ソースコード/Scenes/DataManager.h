//--------------------------------------------------------------------------------------
// File: DataManager.h
//
//　他のクラスから共通で使う変数の受け渡しを担当するシングルトンクラス
//  所持魔力　所持結晶　倒した敵の数　設置しているアルケミカルマシンの総数/種類別
// 
// Date: 2023.6.14
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class DataManager
{
public:
	~DataManager() = default;

	DataManager(const DataManager&) = delete;
	DataManager& operator=(const DataManager&) = delete;

	//static void Create();
	//static void Destroy();

	//static DataManager& GetInstance() { return *instance; }

	//	インスタンスの確保
	static DataManager* GetInstance() { return instance; }

	//	インスタンスの生成
	static void Create() { if (instance == nullptr) instance = new DataManager; }

	//	インスタンスの削除
	static void Destroy()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	void Initialize();
	void Update();

// ゲッター　セッター
public:

	//魔力総量
	int GetNowMP()					const { return m_nowMP; }
	void SetNowMP(int value)			  { m_nowMP = value; }
	int GetNowMP_MAX()				const { return m_nowMP_MAX; }
	void SetNowMP_MAX(int value)		  { m_nowMP_MAX = value; }

	// 最大魔力総量の再計算
	void MPMAXRecalculation(int lv);

	//結晶総量
	int GetNowCrystal()				const { return m_nowCrystal; }
	void SetNowCrystal(int value)		  { m_nowCrystal = value; }
	int GetNowCrystal_MAX()			const { return m_nowCrystal_MAX; }
	void SetNowCrystal_MAX(int value)	  { m_nowCrystal_MAX = value; }

	// 最大クリスタル総量の再計算
	void CrystalMAXRecalculation(int lv);

	//HP総量
	int GetNowBaseHP()				const { return m_nowBaseHp; }
	void SetNowBaseHP(int hp)			  { m_nowBaseHp = hp; }
	int GetNowBaseHP_MAX()			const { return m_nowBaseHp_MAX; }
	void SetNowBaseHP_MAX(int hp)		  { m_nowBaseHp_MAX = hp; }

	// 最大クリスタル総量の再計算
	void BaseHPMAXRecalculation(int lv);

	//敵討伐数
	int GetNowEnemyKill()		const	{ return m_nowEnemyKill; }
	void SetNowEnemyKill(int value)		{ m_nowEnemyKill = value; }

	// ラウンド数
	int GetRound()				const	{ return m_round; }
	void SetRound(int value)			{ m_round = value;}

	// ステージ番号
	int GetStageNum()			const { return m_stageNum;}
	void SetStageNum(int stageNum) { this->m_stageNum = stageNum; }

	int GetStageMax()			const { return 10;}


private:
	DataManager();
	static DataManager* instance;

	// 現在の魔力総量
	int m_nowMP;

	// 現在の魔力最大量
	int m_nowMP_MAX;

	// 現在の結晶総量
	int m_nowCrystal;

	// 現在の結晶最大量
	int m_nowCrystal_MAX;

	// 現在の拠点HP
	int m_nowBaseHp;
	int m_nowBaseHp_MAX;

	// 現在の敵討伐数
	int m_nowEnemyKill;

	// 現在のラウンド数
	int m_round;

	// 現在のステージ数
	int m_stageNum;

	// ステージをクリアしたフラグ
	bool m_stageClearFlag;

};