#include "pch.h"
#include "BulletDataBase.h"
#include "NecromaLib/GameData/CommonStruct.h"
#define MaxLv	5

Bullet_Data RecalculationStatus(ELEMENT element, int lv)
{
	//発射間隔以外　元ステータス * lv / MaxLv + 2
	//発射間隔		(1 + 元ステータス) * MaxLv / lv - 1
	Bullet_Data status = ShareJsonData::GetInstance().GetBulletData(element);

	// 攻撃力
	status.str		= status.str	 * lv / MaxLv;
	// 生存時間
	status.life		= status.life	 * lv / MaxLv;
	// スピード
	status.speed	= status.speed	 * lv / MaxLv;
	// 消費魔力
	status.lossMp	= status.lossMp  * lv / MaxLv;
	// 発射間隔
	status.span		= status.span * lv / MaxLv;

    return status;
}
