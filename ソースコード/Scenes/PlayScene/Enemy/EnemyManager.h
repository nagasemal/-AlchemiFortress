//--------------------------------------------------------------------------------------
// File: EnemyManager.h
//
// エネミーのマネージャー
//  
// 
// Date: 2023.6.7
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "EnemyObject.h"
#include "Scenes/PlayScene/Shadow/Particle.h"
#include <list>

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	/// <summary>
	/// 開始,初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="basePos">ターゲットとなる地点</param>
	void Update(SimpleMath::Vector3 basePos);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// エネミーの上部に表示するビルボードされたアイコンを出す処理
	/// </summary>
	void RenderUI();
	
	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// エネミーのデータを再度読み取る
	/// </summary>
	void ReloadEnemyData();

	/// <summary>
	/// エネミーを生成する
	/// </summary>
	/// <param name="type">エネミーの種類</param>
	/// <param name="spawnNumber">出現順番</param>
	/// <returns></returns>
	EnemyObject CreateEnemy(ENEMY_TYPE type,int spawnNumber);

	/// <summary>
	/// エネミーをランダムで規定数生成する
	/// </summary>
	/// <returns></returns>
	EnemyObject GetRandomEnemy();

	/// <summary>
	/// マシンと当たった際の処理
	/// </summary>
	/// <param name="alchemicalMachines">マシンのポインタ</param>
	void HitAMObejct(AlchemicalMachineObject* alchemicalMachines);

	/// <summary>
	/// エネミーが倒された際に得た経験値
	/// </summary>
	/// <returns></returns>
	int GetNockDownEnemyExp();

	/// <summary>
	/// 動きのコマンドを生成する
	/// </summary>
	/// <param name="moveName">動きの名前</param>
	/// <returns></returns>
	ICommand_Enemy* CreateEnemyMoveCommand(const std::string moveName);

public:


	const int GetKnokDownEnemyFlag() const { return m_knokDownFlag; }
	const ENEMY_TYPE GetKnokDownEnemyType() const { return m_knokDownEnemyType; }
	std::list<EnemyObject>* GetEnemyData() { return m_enemyObject.get(); }

private:

	std::unique_ptr<std::list<EnemyObject>> m_enemyObject;

	//===後で消すテスト用変数
	std::unique_ptr<DirectX::GeometricPrimitive> m_testBox;	//  仮置き四角モデル

	std::unique_ptr<DirectX::Model> m_enemyModel;

	// パーティクルクラス
	std::unique_ptr<Particle> m_particle_spawn;
	std::unique_ptr<Particle> m_particle_delete;

	// ラウンド進行
	float m_nextEnemyTime;

	float m_timer;
	float m_totalTimer;

	// 倒された瞬間を取得するフラグ(1F内に複数体倒すと正常な処理を行えない為、int型で管理)
	int m_knokDownFlag;
	ENEMY_TYPE m_knokDownEnemyType;

	// 1F間に稼いだEXPの総量
	int m_falmeTotalEnemyExp;

	int m_enemyNums;

	// エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

};