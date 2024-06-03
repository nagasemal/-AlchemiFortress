//--------------------------------------------------------------------------------------
// File: EnemyObject.h
//
//  エネミーの内部処理　親クラス
//  ステータスを変えることで種類を持たせる
// 
// Date: 2023.6.7
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "NecromaLib/GameData/CommonStruct.h"

#include "Scenes/PlayScene/Enemy/State/EnemyState.h"

class EnemyCommander;
class EnemyStateContext;
class Enemy_StanderMove;
class Enemy_MeanderingMove;
class ICommand_Enemy;
struct Enemy_Data;
class EnemyManager;

class EnemyObject : public GameObjct3D
{
public:
	/// <summary>
	/// エネミーを生成します
	/// </summary>
	/// <param name="type">エネミーの種類</param>
	/// <param name="startPos">初期位置</param>
	/// <param name="lv">強さ</param>
	/// <param name="spawnTime">出現までの時間</param>
	EnemyObject(ENEMY_TYPE type, SimpleMath::Vector3 startPos,int lv,float spawnTime);
	~EnemyObject();

	// 初期化
	void Initialize()					override;

	// 更新
	void Update()						override;

	// 描画　(UI関係)
	void Draw()							override;

	void Render(Model* model);

	// 終了処理
	void Finalize()						override;

	// 進まないようにする処理
	void HitMachine(bool flag);

	// バウンドさせる
	void Bouns();

// アクセサ
public:

	// 体力
	void SetHp(int hp)									{ m_hp = hp; }

	// ターゲット
	void SetTargetPos(SimpleMath::Vector3 targetPos)	{ m_targetPos = targetPos; }

	//　目的地までの距離
	void SetLengthVec(SimpleMath::Vector3 lengthVec)	{ m_lengthVec = lengthVec; }

	//　止まる処理
	void SetStopFlag(bool flag)							{ m_stopFlag = flag;}

	void SetAliveTimer(float aliveTime)					{ m_aliveTimer = aliveTime; }

	//　加速度
	void SetAccele(SimpleMath::Vector3 accele)			{ m_accele = accele; }

	//　加速度加算
	void AddAccele(SimpleMath::Vector3 accele)			{ m_accele += accele; }

	//　回転設定
	void SetRotate(SimpleMath::Quaternion rotation)		{ m_rotation = rotation; }

	//　死亡フラグ
	void SetDethFlag(bool flag) { m_dethFlag = flag; }

	//　ステータスを設定する
	void SetEnemyData(Enemy_Data data,EnemyManager* manager);

	//　弾に当たった際の処理
	void HitBullet(SimpleMath::Vector3 vectol,int power);

	//　レベル
	const int GetLv()									const{ return m_lv; }
														
	//　体力											
	const int GetHp()									const{ return m_hp; }
														
	//　EXP											   
	const int GetEXP()									const{ return m_exp; }
														
	//　パワー											
	const float GetPower()								const{ return m_power;}
														
	//　とまっているフラグ							  	
	const bool GetStopFlag()							const{ return m_stopFlag; }

	//  出現させる時間　/　生存した時間
	const float GetAliveTimer()							const { return m_aliveTimer; }
														
	//　目的地の位置									
	const SimpleMath::Vector3 GetTargetPos()			const{ return m_targetPos;}
														
	//　目的地までの距離								
	const SimpleMath::Vector3 GetLengthVec()			const{ return m_lengthVec;}

	//  回転
	const SimpleMath::Quaternion GetRotate()			const { return m_rotation;}

	const SimpleMath::Color GetColor()					const { return m_color; }

	//　
	const SimpleMath::Matrix GetModleMatrix()			const { return m_modelMatrix; }
														
	//　加速度											
	const SimpleMath::Vector3 GetAccele()				const{ return m_accele;}
														
	//　自身のエネミータイプ							
	const ENEMY_TYPE GetEnemyType()						const{ return m_enemyType; }

	const bool GetMoveType()							const { return m_moveType; }

	const bool GetDethFlag()							const { return m_dethFlag; }

	// 重力をかからなくする
	void GravityReset() { m_gravityScale = 0.0f; }


	EnemyObject* GetEnemyObject()						{ return this;}

	EnemyCommander* GetEnemyCommand()					{ return m_commander.get();}

private:

	// 攻撃力
	float m_power;
	// HP
	int m_hp;
	// 現在レベル
	int m_lv;
	// 経験値
	int m_exp;
	// 生存している時間 / 出現したのちの時間
	float m_aliveTimer;
	// 移動を止める
	bool m_stopFlag;
	// 属性
	ELEMENT m_element;

	// 動きの種類　true 記述されている動きを全て同時に動かす　false 一つずつ動きを動かす
	bool m_moveType;
	// 自身のタイプ
	ENEMY_TYPE m_enemyType;

	SimpleMath::Color m_color;

	// 加速度
	SimpleMath::Vector3 m_accele;

	// 重力加速度
	float m_gravityScale;

	SimpleMath::Vector3 m_moveVec;

	// 回転
	SimpleMath::Quaternion m_rotation;

	SimpleMath::Matrix m_modelMatrix;

	// 目的地までの距離
	SimpleMath::Vector3 m_lengthVec;

	// 目標の位置
	SimpleMath::Vector3 m_targetPos;

	// コマンドクラスを保存しておく可変長配列
	std::vector<ICommand_Enemy*> m_moveCommands;
	// 動きのデータが入るコマンド
	std::shared_ptr<EnemyCommander> m_commander;

	// エネミーの状態を管理するクラス
	std::shared_ptr<EnemyStateContext> m_stateContext;

	// 死んだことを通知する変数
	bool m_dethFlag;

};