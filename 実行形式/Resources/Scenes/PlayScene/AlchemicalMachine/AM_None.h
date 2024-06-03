//--------------------------------------------------------------------------------------
// File: AM_None.h
//
//　効果無しのアルケミカルマシン
//  仮定義で使う
// 
// Date: 2023.5.15
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "AlchemicalMachineObject.h"
#include <Scenes/PlayScene/Mouse/MousePointer.h>

class AM_None : public AlchemicalMachineObject
{
public:
	AM_None();
	~AM_None();

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

public:

private:

};