#include "pch.h"
#include "MissonAnimation.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/DeltaTime.h"

MissionAnimation::MissionAnimation():
	m_animationFlag(),
	m_animTime()
{
}

MissionAnimation::~MissionAnimation()
{
}

void MissionAnimation::Update()
{

	m_animTime += DeltaTime::GetInstance().GetDeltaTime();

	if (m_animTime >= 1.0f)
	{
		m_animTime = 0.0f;
	}
}

void MissionAnimation::Render()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	pSB->End();
}
