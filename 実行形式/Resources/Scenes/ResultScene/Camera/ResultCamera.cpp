#include "pch.h"
#include "ResultCamera.h"


#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/Animation.h"

#define  CORRECTION_VALUE 0.2f
#define  MAX_SAVEWHELL -3500

#define ROTATION 360.0f

const float ResultCamera::DEFAULT_CAMERA_DISTANCE = 15.0f;

ResultCamera::ResultCamera()
	: m_angleX(0.0f)
	, m_angleY(0.0f)
	, m_prevX(0)
	, m_prevY(0)
	, m_move{ 0.0f,1.0f,0.0f }
	, m_scrollWheelValue(0)
	, m_view(SimpleMath::Matrix::Identity)
	, m_eye(0.0f, 0.0f, 0.0f)
	, m_target{ 0.f }
	, m_animationTimer_Start()
	, m_animationTimer_Select()
	, saveAngleY()
	, m_prevWheelValue()
{
}

ResultCamera::~ResultCamera()
{
}

void ResultCamera::Initialize()
{
	m_angleX = m_angleY = 0.0f;
	m_prevX = m_prevY = 0;
	m_scrollWheelValue = 0;
	m_animationTimer_Start = 0.0f;
	m_animationTimer_Select = 1.0f;
	saveAngleY = 0;

	auto camera = ShareData::GetInstance().GetCamera();

	m_view = camera->GetViewMatrix();
	m_move = SimpleMath::Vector3(0.0f, 1.0f, 0.001f);
	m_eye = camera->GetEyePosition();
	m_target = camera->GetTargetPosition();

}

void ResultCamera::Update()
{
	// 開始時アニメーション
	m_animationTimer_Start += 0.5f * DeltaTime::GetInstance().GetDeltaTime();
	m_move.z = Easing::EaseInOutExpo(1.0f, 1.5f, m_animationTimer_Start);
	m_angleY = Easing::EaseInOutExpo(1.0f, 2.0f, m_animationTimer_Start);

	m_animationTimer_Select += DeltaTime::GetInstance().GetDeltaTime();
	m_angleX = Easing::EaseOutCirc(saveAngleY, saveAngleY + ROTATION * 4.0f, m_animationTimer_Select);

	// ビュー行列の算出
	CalculateViewMatrix();
}

void ResultCamera::AnimationReset()
{
	saveAngleY = m_angleY;
	m_animationTimer_Select = 0.0f;

}

void ResultCamera::CalculateViewMatrix()
{
	// ビュー行列を算出する
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));
	SimpleMath::Matrix rotX = SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angleX));

	SimpleMath::Matrix rt = rotY * rotX;

	SimpleMath::Vector3    eye(m_move);
	SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);

	eye = SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - (float)m_scrollWheelValue / 100);
	up = SimpleMath::Vector3::Transform(up, rt.Invert());

	m_eye = eye;
	m_target = target;
	m_view = SimpleMath::Matrix::CreateLookAt(eye, target, up);
}
