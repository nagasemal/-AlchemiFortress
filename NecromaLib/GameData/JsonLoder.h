//--------------------------------------------------------------------------------------
// File: JsonLoder.h
//
// Jsonを読み書きするクラス
// 
// Date: 2023.7.21
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include <string>
#include <unordered_map>

#include "NecromaLib/GameData/CommonStruct.h"
#include "NecromaLib/GameData/Particle_2D.h"

//	jsonファイルから読みだしたデータを格納する構造体
struct Bullet_Data
{
	std::string element = "Nomal";	//  属性
	float str = 0.0f;		//	攻撃力
	float speed = 0.0f;		//	攻撃の速度
	float span = 0.0f;		//　攻撃の間隔
	float life = 0.0f;		//　弾の生存時間
	int lossMp = 0;		//  減らす魔力
};

struct Machine_Data
{
	std::string element = "Nomal";		// 属性
	int lv = 0;		// Lv
	int hp = 0;		// Hp
	float multiplier_hp = 0.0f;		// レベルアップ時のHPの上昇幅
	float effect_rage	= 0.0f;		// 効果範囲
	float effect_rage_rate	= 0.0f;		// レベルアップ時の効果範囲上昇幅
	float multiplier_effect	= 0.0f;		// レベルアップ時の効果範囲の上昇幅
	int alchemi_mp			= 0;		// 製造時に使うMPの量
	int alchemi_crystal		= 0;		// 製造時に使う魔力の量
	int repea_crystal		= 0;		// 修繕時に使うクリスタルの量
	int lvUp_crystal		= 0;		// レベルアップ時に使うクリスタルの量
	int dismantling_crystal = 0;		// 壊した際に得られるクリスタルの量
	float effect_value		= 0.0f;		// 効果値
};

struct Enemy_MoveData
{
	std::string moveName = "Standerd";
	float delay = 0.0f;
	float time	= 0.0f;
	float value = 0.0f;
};

struct Enemy_Data
{
	ELEMENT element = ELEMENT::NOMAL;
	ENEMY_TYPE type = ENEMY_TYPE::ENMEY_NONE;
	std::string moveType = "ALL";
	int hp		= 1;
	float power = 1.0f;
	int exp		= 1;
	SimpleMath::Color color = SimpleMath::Color();

	std::vector<Enemy_MoveData> moveData;
};

// ステージのクリア条件を格納
struct Stage_Condition
{
	std::string condition = "None";
	int value	 = -1;		//目標値
	int progress = 0;		//進行度
};

struct Stage_Resource
{
	int mp = 400;
	int crystal = 215;
	int hp = 50;
	int exp = 0;
	int lv = 1;
	int attacker = 0;
	int upper = 0;
	int deffencer = 0;
	int mining = 0;
	int recovery = 0;
};

// ステージをクリアしたマシンの情報を格納
struct Stage_Machine
{
	MACHINE_TYPE type = MACHINE_TYPE::NONE;
	ELEMENT element = ELEMENT::NOMAL;
	int lv = 1;
	int number = 0;
};

// エネミーを出現させる時間
struct Enemys_Spawn
{
	ENEMY_TYPE type = ENEMY_TYPE::ENMEY_NONE;	// エネミーのタイプ
	float	spawnTime = 0.0f;					// 召喚するまでの時間
	SimpleMath::Vector3 spawnPos = SimpleMath::Vector3();	// 召喚場所
	int		lv = 1;								// 出現するエネミーのレベル
	bool	condition = false;					// 召喚を開始するタイミング
};

struct Tutorial_Status
{
	int type;		// チュートリアルの処理内容
	int val;		// 処理内容に対して付加する数値
};



struct Stage_Data
{
	std::vector<Stage_Condition>		condition[MISSION_TYPE::MISSION_NUM];			// ステージのクリア条件

	std::vector<Enemys_Spawn>			enemys_Spawn;				// エネミーの出現情報

	Stage_Resource						resource;					// 初めから所持しているリソース群
	std::vector<Stage_Machine>			machine;					// 初めから設置されているマシン群

	std::vector<SimpleMath::Vector2>	crystalPos;					// クリスタルの番号

	std::vector<Tutorial_Status>		tutorial;					// チュートリアル番号
	bool								lastWave = true;		// 次にステージが控えているか	
};

struct Stage_ClearData
{
	std::vector<Stage_Machine> machines;		// クリアしたマシン
	int clearTime = 0;						// クリアまでにかかった時間
	int num = 0;						// 挑戦回数
};

struct UI_Data
{
	SimpleMath::Vector2 pos;							// ポジション
	SimpleMath::Vector2 rage;							// 大きさ
	std::unordered_map<std::string, float> option;		// 追加記述の値
	std::vector<Keyboard::Keys> key;						// キーコード
};

struct Game_Parameter
{
	float rotateSpeed = 0.5f;		// マシンが拠点を回るスピード

	int stage_Max = 1;		// ステージの最大数

	int baseLV_MAX = 9;		// 最大LV

	int baseHp_Max = 0;		// 拠点HPの最大LVの値

	int mp_Max = 0;		// MPリソースの最大LVの値

	int crystal_Max = 0;		// クリスタルリソースの最大LVの値

	int needExp = 0;		// 必要な経験値の量
	int needExp_Growthrate = 0;		// 必要な経験値の量の上がり幅

	float transparent_time = 0;		// 半透明化を開始する秒数
	float transparent_val = 0;			// 半透明化の数値
};

struct Particle2D_Pram
{
	ParticleUtility_2D praticleData;
	ParticleUtility_2D random_Max;

	int num = 0;
	int maxNum = 0;

	float rage = 0.0f;
	float maxRage = 0.0f;

	float time = 0.0f;

	std::vector<SimpleMath::Vector2> points;				// 生成箇所の拡張
	float linerSpeed	= 0.0f;
	float maxLinerSpeed = 0.0f;

	std::string tag;		// パーティクルを示すタグ
	Particle_2D::ParticleDrawType	drawType = Particle_2D::ParticleDrawType::DrawNone;
	Particle_2D::ParticleLinerType	linerType = Particle_2D::ParticleLinerType::LinerNone;		// ポイント間の生成の仕方
	Particle_2D::ParticleMoveType	velocityType = Particle_2D::ParticleMoveType::MoveNone;		// 速度の加算方法
	Particle_2D::ParticleMoveType	acceleType = Particle_2D::ParticleMoveType::MoveNone;		// 加速度の加算方法
};


// 書き込み
class AlchemicalMachineObject;
class ICommand_Enemy;

namespace Json
{
	// 読み込み：Attackerが射出する弾の情報
	Bullet_Data FileLoad_BulletData(const std::string filePath);
	// 読み込み：マシンの基本データ
	Machine_Data FileLoad_MachineData(const std::string filePath);
	// 読み込み：エネミーの基本データ
	Enemy_Data FileLoad_EnemyData(const std::string filePath);
	// 読み込み：ステージの基本データ
	Stage_Data FileLoad_StageData(const std::string filePath);
	// 読み込み：ステージクリア情報
	Stage_ClearData FileLoad_StageClearData(const std::string filePath);
	// 読み込み：ゲーム内パラメータ情報
	Game_Parameter FileLoad_GameParameter(const std::string filePath);
	// 読み込み：UI位置の情報
	UI_Data FileLoad_UIData(const std::string filePath);
	// 読み込み：パーティクル2Dの値
	Particle2D_Pram FileLoad_Particle2D(const std::string filePath);

	void WritingJsonFile_ClearData(int number,std::vector<std::shared_ptr<AlchemicalMachineObject>> alchemicalMachineList,int time);

	// エディターに使用　ステージのデータを書き込む
	void WritingJsonFile_StageData(int number,Stage_Data stageData);

	// ステージのクリアデータを初期化する
	void InitializationClearStageData();
}