#pragma once
#include "NecromaLib/GameData/GameObject3D.h"

class FieldObject : public GameObjct3D
{
public:
	const enum OBJECT_TYPE : int
	{
		NONE = 0,				// 指定なし
		CRYSTAL = 1,			// クリスタル
		ENEMY_SPAWNER = 2,		// エネミースポナー
	};
public:
	FieldObject();
	~FieldObject();

	virtual void Render(DirectX::Model* model) = 0;

	virtual void WriteDepth(DirectX::Model* model) = 0;

protected:

};