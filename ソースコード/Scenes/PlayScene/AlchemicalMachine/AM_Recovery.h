//--------------------------------------------------------------------------------------
// File: AM_Recovery.h
//
//　範囲内のアルケミカルマシンに対して
//  強化効果を発揮する
// 
// Date: 2023.5.29
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include <Scenes/PlayScene/Mouse/MousePointer.h>

class AM_Recovery : public AlchemicalMachineObject
{
public:
	AM_Recovery();
	~AM_Recovery();

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

	void HitEnemy(std::list<EnemyObject>* enemy);

	void MPPuls(DataManager* pDM);

private:

};