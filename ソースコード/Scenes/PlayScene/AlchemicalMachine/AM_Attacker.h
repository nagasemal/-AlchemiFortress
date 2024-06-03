//--------------------------------------------------------------------------------------
// File: AM_Attacker.h
//
//　エネミーを攻撃するアルケミカルマシン
// 
// Date: 2023.5.15
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------


#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/BulletDataBase.h"

class AM_Attacker final : public AlchemicalMachineObject
{
public:

	AM_Attacker();
	~AM_Attacker();

	// 初期化
	void Initialize()					override;

	// 更新
	void Update()						override;

	// 選択時に更新される処理
	void SelectUpdate()					override;

	// 描画　(UI関係)
	void Draw()							override;

	// 終了処理
	void Finalize()						override;

	void RenderUI() override;

	// 射出のリクエストを出す
	bool BulletRequest(std::list<EnemyObject>* enemys);

	// 
	Bullet GetBulletData();

//　アクセサ
public:

	const Bullet_Data GetBulletStatus()	const { return m_bulletStatus;}

private:

	float	m_timer;

	Bullet_Data m_bulletStatus;		// バレットデータ
	Bullet_Data m_saveBulletStatus;	// 加工元のバレットデータ

	SimpleMath::Vector3 m_targetPos;

	// 選択可能オブジェクト
	std::unique_ptr<SelectionBox> m_selectBox[4];
};