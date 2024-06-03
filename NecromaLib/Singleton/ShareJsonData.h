//--------------------------------------------------------------------------------------
// File: ShareJsonData.h
//
// jsonで読み込んだ数値を渡す
//
// 内容物
// ・バレットのパラメータ
// ・マシンのパラメータ
// ・エネミーのパラメータ
// ・ゲームに関する固定の変数
// ・ステージ情報
// ・ステージクリア所法
// ・UIの位置情報
// ・2Dパーティクルの情報
// 
// Date: 2023.7.21
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once

#include "NecromaLib/GameData/JsonLoder.h"

class ShareJsonData
{
public:
	~ShareJsonData()								= default;

	ShareJsonData(const ShareJsonData&)				= delete;
	ShareJsonData& operator=(const ShareJsonData&)	= delete;

	static void Create();
	static void Destroy();

	static ShareJsonData& GetInstance() { return *instance; }

	// バレットのjsonファイルを読み込む
	void LoadingJsonFile_Bullet();

	// マシンのjsonファイルを読み込む
	void LoadingJsonFile_Machine();

	// エネミーのjsonファイルを読み込む
	void LoadingJsonFile_Enemy();

	// ゲームに関する固定の変数(リソースの最大値など)を読み込む
	void LoadingJsonFile_GameParameter();

	// ステージのjsonファイルを読み込む
	void LoadingJsonFile_Stage(int number, int wave);

	// クリアデータのjsonファイルを読み込む
	void LoadingJsonFile_ClearData(int number);

	// UIのレイアウトを取得する
	void LoadingUIData(std::string pathName,std::string tagName);

	// パーティクル2Dのデータを取得する
	void LoadingParticle2D(std::string pathName);

	// エンドレスモード時のみ使用(次のステージ内容を変える)
	void SetEndlessStageData(Stage_Data data);

public:

	// 属性に対応したバレットのパラメータを渡す
	const Bullet_Data GetBulletData(ELEMENT element);

	// マシンタイプに対応したマシンのパラメータを渡す
	const Machine_Data GetMachineData(MACHINE_TYPE type);

	// エネミータイプに対応したエネミーのパラメータを渡す
	const Enemy_Data GetEnemyData(ENEMY_TYPE type);

	// ステージのデータを渡す
	const Stage_Data GetStageData();

	// ステージのクリアデータを渡す
	const Stage_ClearData GetClearData();

	// ゲーム内のパラメータを渡す
	const Game_Parameter GetGameParameter();

	// UIの配置を渡す
	const UI_Data GetUIData(std::string tagName);

	// パーティクル2Dのデータを渡す
	const Particle2D_Pram GetParticle2DPram(std::string tagName);

	// ステージデータの中身を全て消す(メモリ領域の圧迫を防ぐため)
	void StageDataCleanUP();

	// エレメントに紐づく色情報を取得
	SimpleMath::Color GetElementColor(ELEMENT element);


private:
	ShareJsonData();
	static ShareJsonData* instance;

	// Attackerが繰り出す弾のパラメータ
	Bullet_Data m_bulletData[ELEMENT::Num];

	// 各マシンのデータ
	Machine_Data m_machineData[MACHINE_TYPE::NUM];

	// 各エネミーのデータ
	Enemy_Data m_enemyData[ENEMY_TYPE::ENEMY_NUM];

	// 読み込んだステージのデータ(全てを読み込むとメモリ効率が悪い為、随時読み込む方式を採用)
	Stage_Data m_stageData;

	// 読み込んだステージのクリア時データ
	Stage_ClearData m_clearData;

	// ゲーム内パラメータ
	Game_Parameter m_gameParam;

	// 2Dパーティクルの情報を持った配列
	std::unordered_map<std::string, Particle2D_Pram> m_particlePrams;

	// UIの位置大きさその他情報を持った配列
	std::unordered_map<std::string, UI_Data> m_uiDatas;

};