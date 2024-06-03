//--------------------------------------------------------------------------------------
// File: MousePointer.h
//
// プレイシーン上で扱う。現在のマウス位置を板ポリゴンで表示
//
//
// Date: 2023.5.11
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject3D.h"

class Particle;

class MousePointer : public GameObjct3D
{
public:
	MousePointer();
	~MousePointer();

	// 初期化
	void Initialize()	override;

	// 更新
	void Update()		override;

	// 描画
	void Draw()			override;

	void ModelDraw(DirectX::Model* model);

	// 終了処理
	void Finalize()		override;

public:

	void HitMachine(SimpleMath::Vector3 pos);

	// オブジェクトを動かす際に動く処理
	void ObjectDragMode();

	// オブジェクトを離した際に動く処理
	void ReleaseLeftButtom();

private:

	std::unique_ptr<DirectX::Model>				m_mouseCursorModel;

	// パーティクルクラス
	std::unique_ptr<Particle> m_particle;

	float m_time;



};