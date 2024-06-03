//--------------------------------------------------------------------------------------
// File: Field.h
//
// フィールドの情報を保有
// 敵がでる洞穴　クリスタル　保有
//
//  
// Date: 2023.4.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "Scenes/PlayScene/Field/Crystal/Crystal.h"
#include "Scenes/PlayScene/Field/FieldObjectType.h"

class Field : public FieldObject
{

public:
	Field();
	~Field();

	// 初期化
	void Initialize()	override;

	// 更新
	void Update()		override;

	// 描画
	void Draw()			override;

	void Render(DirectX::Model* model) override;

	void WriteDepth(DirectX::Model* model) override;

	// 終了処理
	void Finalize()		override;

private:

/*===[　アクセサ　]===*/
public:

	bool		GetHitMouse()		const	{ return m_hitMouseFlag; }

private:

	// マウスとの衝突時
	bool		m_hitMouseFlag;

};