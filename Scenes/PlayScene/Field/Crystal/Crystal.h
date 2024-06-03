//--------------------------------------------------------------------------------------
// File: Crystal.h
//
// リソース群　MiningTypeのアルケミカルマシンが触れると結晶リソースが増える
//
//  
// Date: 2023.6.13
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "Scenes/PlayScene/Field/FieldObjectType.h"

class Crystal : public FieldObject
{
public:
	Crystal(SimpleMath::Vector3 pos,float rotateY);
	~Crystal();

	// 初期化
	void Initialize()					override;

	// 更新
	void Update()						override;

	// 描画
	void Draw()							override;

	// 終了処理
	void Finalize()						override;

	// モデル描画
	void Render(Model* model)			override;

	void WriteDepth(DirectX::Model* model) override;

	// 親クラスに消す処理を行わせる
	bool DeleteRequest();

	bool GetHitMouse();

public:

	// オブジェクトタイプを渡す
	OBJECT_TYPE GetObjectType() { return m_type; }

private:

	float m_rotateY;

	bool m_selectFlag;

	OBJECT_TYPE m_type;
	
};