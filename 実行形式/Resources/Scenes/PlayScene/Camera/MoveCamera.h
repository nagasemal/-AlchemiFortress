//--------------------------------------------------------------------------------------
// File: MoveCamera.h
//
//　プレイシーンのカメラ挙動
// 
// Date: 2023.7.12
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "NecromaLib/GameData/Animation.h"

class MoveCamera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// カメラの距離

	MoveCamera();
	~MoveCamera();

	void Initialize();
	// 更新処理
	void Update(bool scroll,bool move);

	// ビュー行列を取得する
	SimpleMath::Matrix GetViewMatrix() const		{ return m_view; }
	// デバッグカメラの座標を取得する
	SimpleMath::Vector3 GetEyePosition() const		{ return m_eye; }
	// デバッグカメラの注視点座標を取得する
	SimpleMath::Vector3 GetTargetPosition() const	{ return m_target; }
	// ゲーム開始時の引きが終わったことを知らせるフラグ
	bool GetStopCameraFlag();

	void SetEyeProsition(SimpleMath::Vector3 eye)			{ m_eye = eye;}
	void SetTargetProsition(SimpleMath::Vector3 target)		{ m_target = target; }

	void SetCameraMove(SimpleMath::Vector3 position)		{ m_move = position; }

	void SetCameraLock(bool flag) { m_moveLock = flag; }
	bool GetCameraLock() { return m_moveLock; }

	void TargetChange(SimpleMath::Vector3 target);
	void ResetTargetChangeTimer();

private:

	// 縦回転角
	float m_angleX;								
	// 横回転角
	float m_angleY;								
	// 前回のマウス座標(X,Y)
	int m_prevX, m_prevY;						
	// マウスホイールのスクロール値
	int m_scrollWheelValue;				
	// 前回のスクロール値
	int m_prevWheelValue;						
	// 移動値
	SimpleMath::Vector3 m_move;		
	// 生成されたビュー行列
	SimpleMath::Matrix m_view;			
	// カメラの座標
	SimpleMath::Vector3 m_eye;
	// 注視点
	SimpleMath::Vector3 m_target;

	// 次の注視点
	SimpleMath::Vector3 m_nextTarget;

	// 引きで使うタイマー
	AnimationData m_time;			
	// ターゲット変更に使うタイマー
	AnimationData m_targetChangeTime;

	// マウスポインタのドラッグ開始位置からの変位(相対値)
	void DraggedDistance(int x, int y);

	// ビュー行列の算出
	void CalculateViewMatrix();

	bool m_moveLock;
};