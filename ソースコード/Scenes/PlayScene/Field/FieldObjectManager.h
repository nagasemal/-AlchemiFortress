//--------------------------------------------------------------------------------------
// File: FieldObjectManager.h
//
//　フィールド上に設置される回転しないオブジェクトの管理
//
//  
// Date: 2023.6.14
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "Scenes/PlayScene/Field/Field.h"
#include "Scenes/PlayScene/Field/Crystal/Crystal.h"

// ToDo場所変え必須
#include "Scenes/PlayScene/AlchemicalMachine/PlayerBase.h"
#include <list>

#include "Scenes/PlayScene/Shadow/MagicCircle.h"

class EnemyManager;

class FieldObjectManager
{
public:
	FieldObjectManager();
	~FieldObjectManager();

	void Initialize();
	void Update(EnemyManager* pEnemyManager);
	void Draw();
	void DrawBillboardUI();
	void WriteDepath();
	void Finalize();

public:

	// 結晶の生成
	void CrystalSpawn(int num);

	Field* GetFieldObject()						{ return m_field.get();}
	PlayerBase* GetPlayerBase()					{ return m_playerBase.get(); }
	std::list<Crystal>* GetCrystalObject()		{ return m_crystals.get();}

	bool GetCrystalToMouse()					{ return m_crystalTomouseFlag;}

private:

	// エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 床モデル
	std::unique_ptr<DirectX::Model> m_floorModel;
	// 結晶モデル
	std::unique_ptr<DirectX::Model> m_crystalModel;
	// 拠点モデル
	std::unique_ptr<DirectX::Model> m_baseModel_Towor;

	std::unique_ptr<DirectX::Model> m_baseModel_Pillar;

	// フィールドオブジェクト
	std::unique_ptr<Field> m_field;

	// プレイヤーの拠点
	std::unique_ptr<PlayerBase> m_playerBase;

	// 結晶配列
	std::unique_ptr<std::list<Crystal>> m_crystals;

	// クリスタルとマウスの当たり判定
	bool m_crystalTomouseFlag;

};