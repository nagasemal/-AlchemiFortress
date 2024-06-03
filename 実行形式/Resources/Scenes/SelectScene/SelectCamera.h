//--------------------------------------------------------------------------------------
// File: SelectCamera.h
//
//　SelectScene内で使われるカメラクラス
// 
// Date: 2023.8.10
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------


#pragma once

#include "NecromaLib/GameData/Animation.h"

class SelectCamera
{
public:

	static const float DEFAULT_CAMERA_DISTANCE;	// カメラの距離

	SelectCamera();
	~SelectCamera();

	void Initialize();
	// 更新処理
	void Update();

	void AnimationReset();

	// ビュー行列を取得する
	SimpleMath::Matrix GetViewMatrix() const { return m_view; }
	// デバッグカメラの座標を取得する
	SimpleMath::Vector3 GetEyePosition() const { return m_eye; }
	// デバッグカメラの注視点座標を取得する
	SimpleMath::Vector3 GetTargetPosition() const { return m_target; }

	float GetAnimationTimer() { return m_animationTimer_Select;}

private:

	float m_angleX;								// 縦回転角
	float m_angleY;								// 横回転角
	int m_prevX, m_prevY;						// 前回のマウス座標(X,Y)
	int m_scrollWheelValue;						// マウスホイールのスクロール値
	int m_prevWheelValue;						// 前回のスクロール値
	SimpleMath::Vector3 m_move;		// 移動値
	SimpleMath::Matrix m_view;			// 生成されたビュー行列
	SimpleMath::Vector3 m_eye;			// カメラの座標
	SimpleMath::Vector3 m_target;		// 注視点

	AnimationData m_animationTimer_Start;				// 始めに使われるタイマー

	AnimationData m_animationTimer_Select;				// ステージの切り替わりで使われるタイマー

	float saveAngleY;

	// ビュー行列の算出
	void CalculateViewMatrix();

};