#include "pch.h"
#include "TitleCamera.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/Animation.h"

#define  CORRECTION_VALUE 0.2f
#define  MAX_SAVEWHELL -3500


const float TitleCamera::DEFAULT_CAMERA_DISTANCE = 15.0f;

TitleCamera::TitleCamera()
	: m_angleX(0.0f)
	, m_angleY(0.0f)
	, m_prevX(0)
	, m_prevY(0)
	, m_move{ 0.0f,1.0f,0.0f}
	, m_scrollWheelValue(0)
	, m_view(SimpleMath::Matrix::Identity)
	, m_eye(0.0f, 0.0f, 0.0f)
	, m_target{ 0.f }
	, m_animationData()
	, m_prevWheelValue()
{
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Initialize()
{
	m_angleX = m_angleY = 0.0f;
	m_prevX = m_prevY = 0;
	m_scrollWheelValue = 0;
	m_animationData = AnimationData();


	m_view = SimpleMath::Matrix::Identity;
	m_move = SimpleMath::Vector3::Zero;
	m_eye = SimpleMath::Vector3::Zero;
	m_target = SimpleMath::Vector3::Zero;

}

void TitleCamera::Update()
{
	// 開始時アニメーション
		m_animationData += 0.5f * DeltaTime::GetInstance().GetDeltaTime();

		m_target.x = Easing::EaseInOutBack(0.0f, 10.0f, m_animationData);
		m_target.z = Easing::EaseInOutCirc(0.0f, 20.0f, m_animationData);

		m_move.x = Easing::EaseInOutBack(0.0f, 2.0f, m_animationData);
		m_move.z = Easing::EaseInOutBack(0.0f, 2.0f, m_animationData);
		m_move.y = Easing::EaseInOutBack(1.0f, 3.0f, m_animationData);

		//m_angleX = Easing::EaseInOutQuart(0.0f, 55.0f, m_animationTimer);
		m_angleY = Easing::EaseInOutQuart(0.0f, 100.0f, m_animationData);

		// ビュー行列の算出
		CalculateViewMatrix();
}

void TitleCamera::CalculateViewMatrix()
{
	// ビュー行列を算出する
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));
	SimpleMath::Matrix rotX = SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angleX));

	SimpleMath::Matrix rt = rotY * rotX;

	SimpleMath::Vector3    eye(m_move);
	//SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);

	eye = SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - (float)m_scrollWheelValue / 100);
	up = SimpleMath::Vector3::Transform(up, rt.Invert());

	m_eye = eye;
	m_target/* = target;*/;
	m_view = SimpleMath::Matrix::CreateLookAt(eye, m_target, up);
}
