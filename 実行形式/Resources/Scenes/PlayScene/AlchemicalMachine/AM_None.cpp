#include "pch.h"
#include "AM_None.h"

#include "NecromaLib/Singleton/ModelShader.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"

AM_None::AM_None()
{
}

AM_None::~AM_None()
{

}

void AM_None::Initialize()
{
	m_machineID = MACHINE_TYPE::NONE;
	m_objectName = "None";
	m_lv = 1;

}

void AM_None::Update()
{
	m_color = Colors::Black;

	// 選択モード時、色を変える
	if (m_selectModeFlag)
	{
		m_selectModeTime += DeltaTime::GetInstance().GetNomalDeltaTime() * 2.0f;

		float transitionVal = sinf(m_selectModeTime);

		m_color = SimpleMath::Color(transitionVal, transitionVal, transitionVal / 1.25f, 1.0f);
	}
	else
	{
		m_selectModeTime = 0;
	}

	// マウスが触れている間、色を変える
	if (m_hitMouseFlag)
	{
		m_color = Colors::White;
	}

	// レベルのリセット
	m_lv = 1;
}

void AM_None::SelectUpdate()
{
}

void AM_None::Draw()
{
}

void AM_None::Finalize()
{
}

void AM_None::RenderUI()
{
}
