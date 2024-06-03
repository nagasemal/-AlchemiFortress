//
// Camera.h
//
#pragma once

#include <SimpleMath.h>

/// <summary>
/// デバッグ用カメラクラス
/// </summary>
class Camera
{
public:
	static const float DEFAULT_CAMERA_DISTANCE;	// カメラの距離

private:

	// ビュー行列
	SimpleMath::Matrix mView;

	// プロジェクション行列
	SimpleMath::Matrix mProjection;

	// カメラ座標
	SimpleMath::Vector3 mEye;

	// 注視点
	SimpleMath::Vector3 mTarget;

	// 上向きベクトル
	SimpleMath::Vector3 mUp;

public:
	// コンストラクタ
	Camera();

	// デストラクタ
	virtual ~Camera();

	// ビュー行列のアクセサ
	void SetViewMatrix(SimpleMath::Matrix view) { mView = view; }

	SimpleMath::Matrix GetViewMatrix()
	{
		CalculateViewMatrix();
		return mView;
	}

	// プロジェクション行列のアクセサ
	void SetProjectionMatrix(SimpleMath::Matrix projection) { mProjection = projection; }

	void CameraReset();

	SimpleMath::Matrix GetProjectionMatrix()
	{
		CalculateProjectionMatrix();
		return mProjection;
	}

	// カメラ座標のアクセサ
	void SetEyePosition(SimpleMath::Vector3 eye) { mEye = eye; }
	SimpleMath::Vector3 GetEyePosition() const { return mEye; }

	// 注視点座標のアクセサ
	void SetTargetPosition(SimpleMath::Vector3 target) { mTarget = target; }
	SimpleMath::Vector3 GetTargetPosition() const { return mTarget; }

	// 上向きベクトルのアクセサ
	void SetUpVector(SimpleMath::Vector3 up) { mUp = up; }
	SimpleMath::Vector3 GetUpVector() const { return mUp; }

protected:

	// ビュー行列の算出
	virtual void CalculateViewMatrix();

	// プロジェクション行列の算出
	virtual void CalculateProjectionMatrix();
};
