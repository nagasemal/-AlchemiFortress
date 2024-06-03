#include "pch.h"
#include "MoveCamera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"

#define  CORRECTION_VALUE 0.2f
#define  MAX_SAVEWHELL -3500


const float MoveCamera::DEFAULT_CAMERA_DISTANCE = 15.0f;

MoveCamera::MoveCamera()
	: m_angleX(0.0f)
	, m_angleY(0.0f)
	, m_prevX(0)
	, m_prevY(0)
	, m_move{ 0.f }
	, m_scrollWheelValue(0)
	, m_view(SimpleMath::Matrix::Identity)
	, m_eye(0.0f, 0.0f, 0.0f)
	, m_target{ 0.f }
	, m_nextTarget{ 0.f }
	, m_moveLock()
	, m_time()
	, m_targetChangeTime()
	, m_prevWheelValue()
{
}

MoveCamera::~MoveCamera()
{
}

void MoveCamera::Initialize()
{
	m_angleX = m_angleY = 0.0f;
	m_prevX  = m_prevY  = 0;
	m_scrollWheelValue = 0;
	m_time = m_targetChangeTime = AnimationData();

	m_view	    = SimpleMath::Matrix::Identity;
	m_move		= SimpleMath::Vector3::Zero;
	m_eye		= SimpleMath::Vector3::Zero;
	m_target	= SimpleMath::Vector3::Zero;

}

void MoveCamera::Update(bool scroll, bool move)
{
	scroll;

	// カメラの動きを行わないようにする
	if (m_moveLock) return;

	auto state = InputSupport::GetInstance().GetMouseState().GetLastState();
	auto keyboard = InputSupport::GetInstance().GetKeybordState().GetLastState();

	// 開始時引く為の処理
	m_time += DeltaTime::GetInstance().GetDeltaTime();

	m_move.y = m_time * 1.2f;
	m_move.z = m_time * 1.3f;


	m_targetChangeTime += DeltaTime::GetInstance().GetDeltaTime() * 1.15f;

	//m_target = SimpleMath::Vector3::Lerp(m_target,m_saveTarget, m_targetChangeTime);

	// カメラ移動をするか否か
	if (move)
	{
		// ホイール長押しでカメラを移動させる
		if (InputSupport::GetInstance().MiddleButton_Held())
		{
			DraggedDistance(state.x, state.y);
		}

		// マウスの座標を前回の値として保存
		m_prevX = state.x;
		m_prevY = state.y;

	}

	int value = state.scrollWheelValue - m_prevWheelValue;
	int newValue = m_scrollWheelValue + value;

	// 上限下限設定(clamp)
	if (newValue <= MAX_SAVEWHELL) newValue = MAX_SAVEWHELL;
	if (newValue > 0) newValue = 0;

	if (newValue != 0 && newValue != MAX_SAVEWHELL)
	{
		m_scrollWheelValue = newValue;
	}
	else
	{
		value = 0;
	}

	if (m_scrollWheelValue == MAX_SAVEWHELL && value > 0)
	{
		m_scrollWheelValue -= value;
	}
	else if (m_scrollWheelValue == 0 && value < 0)
	{
		m_scrollWheelValue -= value;
	}

	if (m_scrollWheelValue > 0)
	{
		m_scrollWheelValue = 0;
		DirectX::Mouse::Get().ResetScrollWheelValue();
	}

	// 前回の値を保存
	m_prevWheelValue = state.scrollWheelValue;

	// ビュー行列の算出
	CalculateViewMatrix();
}

bool MoveCamera::GetStopCameraFlag()
{
	return m_time.MaxCheck();
}

void MoveCamera::TargetChange(SimpleMath::Vector3 target)
{

	// カメラの動きを行わないようにする
	if (m_moveLock) return;

	m_nextTarget = target;

}

void MoveCamera::ResetTargetChangeTimer()
{

	if (m_targetChangeTime.MaxCheck())
	{
		m_targetChangeTime = 0.0f;
	}

}

void MoveCamera::DraggedDistance(int x, int y)
{
	// マウスポインタの前回からの変位
	float dx = static_cast<float>(x - m_prevX) * CORRECTION_VALUE;
	float dy = static_cast<float>(y - m_prevY) * CORRECTION_VALUE;

	if (dx != 0.0f || dy != 0.0f)
	{
		// マウスポインタの変位を元に、Ｘ軸Ｙ軸の回転角を求める
		// XとYの関係に注意！！
		float angleX = dy * DirectX::XM_PI / 180.0f;
		float angleY = dx * DirectX::XM_PI / 180.0f;

		// 角度の更新 Yには回転の制限をかける
		m_angleX += (m_angleX + angleX) >= 0.8f ? 0 : (m_angleX + angleX) <= -0.7f ? 0 : angleX;
		m_angleY += angleY;
	}
}

void MoveCamera::CalculateViewMatrix()
{
	// ビュー行列を算出する
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_angleY);
	SimpleMath::Matrix rotX = SimpleMath::Matrix::CreateRotationX(m_angleX);

	SimpleMath::Matrix rt = rotY * rotX;

	SimpleMath::Vector3    eye(m_move.x, m_move.y, m_move.z);
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);

	eye = SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - (float)m_scrollWheelValue / 100);
	up = SimpleMath::Vector3::Transform(up, rt.Invert());

	m_eye		= eye;
	m_target	+= (m_nextTarget - m_target) * 0.05;
	m_view		= SimpleMath::Matrix::CreateLookAt(eye, m_target, up);
}
