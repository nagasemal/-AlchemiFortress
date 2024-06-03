//--------------------------------------------------------------------------------------
// File: GameObject2D.h
//
//　2Dオブジェクトのインターフェース
// 
// Author: KazumaNagase
//--------------------------------------------------------------------------------------

#pragma once

#include <SimpleMath.h>

#include "StepTimer.h"
#include "NecromaLib/GameData/Collider.h"
#include <DeviceResources.h>
#include <SpriteBatch.h>
#include "NecromaLib/Singleton/ShareData.h"

using namespace DirectX;

class GameObject2D
{
protected:

	struct ObjectData2D
	{
		SimpleMath::Vector2 pos = SimpleMath::Vector2();
		SimpleMath::Vector2 rage = SimpleMath::Vector2();
	};

	ObjectData2D m_data;

public:
	GameObject2D();
	~GameObject2D();

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// 終了処理
	virtual void Finalize() = 0;

	// ポジションと大きさをもった構造体を渡す
	ObjectData2D GetData() { return m_data; }

	SimpleMath::Vector2 GetRage()	{ return m_data.rage; }
	SimpleMath::Vector2 GetPos()	{ return m_data.pos; }

	void SetPos(SimpleMath::Vector2 pos)	{ m_data.pos = pos;}
	void SetRage(SimpleMath::Vector2 rage)	{ m_data.rage = rage;}

	// 当たる対象のオブジェクト
	bool  HitObject(SimpleMath::Vector2 pos);

	/// <summary>
	/// スプライトバッチでの描画オブジェクトに適した当たり判定
	/// </summary>
	/// <param name="pos">対象のオブジェクトのポジション</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	/// <param name="percent">0 ~ 1の範囲の拡縮率</param>
	/// <returns></returns>
	bool HitObject_RageSet(SimpleMath::Vector2 pos, float width = 64.0f, float height = 64.0f, float percent = 1.0f);

	/// <summary>
/// スプライトバッチでの描画オブジェクトに適した当たり判定
/// </summary>
/// <param name="pos">対象のオブジェクトのポジション</param>
/// <param name="width">横幅</param>
/// <param name="height">縦幅</param>
/// <param name="percent">0 ~ 1の範囲の拡縮率</param>
/// <returns></returns>
	bool HitObject_RageSet(SimpleMath::Vector2 pos, float width = 64.0f, float height = 64.0f, SimpleMath::Vector2 percent = {1.0f,1.0f});

};