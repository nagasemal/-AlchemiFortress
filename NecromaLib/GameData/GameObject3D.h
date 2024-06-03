#pragma once

#include <SimpleMath.h>

#include "StepTimer.h"
#include "NecromaLib/GameData/Collider.h"
#include <DeviceResources.h>
#include <SpriteBatch.h>
#include "NecromaLib/Singleton/ShareData.h"

using namespace DirectX;

class GameObjct3D
{
public:

	/// <summary>
	/// 3Dオブジェクトの位置情報,大きさ情報が格納された構造体
	/// </summary>
	struct ObjectData3D
	{
		SimpleMath::Vector3 pos;
		SimpleMath::Vector3 rage;

		ObjectData3D(SimpleMath::Vector3 pos = SimpleMath::Vector3::Zero,
			SimpleMath::Vector3 rage = SimpleMath::Vector3::Zero)
		{
			this->pos = pos, this->rage = rage;
		}
	};

protected:

	ObjectData3D m_data;

	bool m_colliderActive;



public:
	GameObjct3D();
	~GameObjct3D();

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// 終了処理
	virtual void Finalize() = 0;

	/// <summary>
	/// この3Dオブジェクトの大きさと位置が格納された構造体を返します
	/// </summary>
	/// <returns></returns>
	ObjectData3D GetData()							{ return m_data; }

	/// <summary>
	/// GameObject3Dにダウンキャストされたクラスを返します。
	/// </summary>
	GameObjct3D* GetObject3D()						{ return static_cast<GameObjct3D*>(this);}

	/// <summary>
	/// この3Dオブジェクト大きさを返します
	/// </summary>
	/// <returns></returns>
	SimpleMath::Vector3 GetRage()					{ return m_data.rage; }

	/// <summary>
	/// この3Dオブジェクトワールド空間座標を返します
	/// </summary>
	/// <returns></returns>
	SimpleMath::Vector3 GetPos()					{ return m_data.pos; }

	/// <summary>
	/// 当たり判定の処理を行うか決めます　
	/// </summary>
	/// <param name="flag">TRUE : 行う　FALSE : 行わない</param>
	void SetColliderActive(bool flag)				{ m_colliderActive = flag; }

	/// <summary>
	/// 当たり判定の処理を行うかのフラグを返します
	/// </summary>
	/// <returns></returns>
	bool GetColliderActive()						{ return m_colliderActive; }

	/// <summary>
	/// この3Dオブジェクトが他の3Dオブジェクトに触れているかを検知します
	/// </summary>
	/// <param name="data">他の3Dオブジェクト</param>
	/// <returns></returns>
	int  HitObject_Box(GameObjct3D* data);

	/// <summary>
	/// この3Dオブジェクトが他の3Dオブジェクトに触れているかを検知します
	/// </summary>
	/// <param name="data">他の3Dオブジェクト</param>
	/// <returns></returns>
	bool HitObject_Circle(GameObjct3D* data);

	/// <summary>
	/// この3Dオブジェクトの大きさから円周範囲を求めます
	/// </summary>
	/// <returns></returns>
	Circle GetCircle()								{ return Circle(m_data.pos, m_data.rage.x * 1.5f); }

};