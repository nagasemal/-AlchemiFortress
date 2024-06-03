//--------------------------------------------------------------------------------------
// File: BulletDataBase.h
//
//　バレットのステータスを全て保持している
//  LvUpによる再計算も可　Lv5が最大
// 
// Date: 2023.6.16
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/Singleton/ShareJsonData.h"

	// ステータスの再計算
	Bullet_Data RecalculationStatus(ELEMENT element,int lv);