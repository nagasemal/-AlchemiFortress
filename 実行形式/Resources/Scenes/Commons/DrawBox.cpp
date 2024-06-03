#include "pch.h"
#include "DrawBox.h"

#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/Commons/DrawClick.h"
#include "NecromaLib/GameData/Particle_2D.h"

#include "NecromaLib/Singleton/InputSupport.h"

#define MAX_SIZE 50.0f
#define MIN_SIZE 3.0f

DrawBox::DrawBox(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage, float lineRage)
{

	m_animationData_First = AnimationData(0.0f,0.0f,1.0f,0.8f);

	m_lineRage_Vertical = m_lineRage_Beside = SimpleMath::Vector2();
	m_lineRage_Vertical.x = m_lineRage_Beside.y = lineRage;

	m_animationData_Second = AnimationData();
	m_rotate_easing = 0.0f;

	m_saveData.pos = m_boxPos = m_data.pos = pos;
	m_saveData.rage = m_data.rage = rage;

	m_color = Colors::Black;

	m_rect = RECT{ 1,1,2,2 };

	m_animationFlag = false;

	m_clickPointer = std::make_unique<DrawClick>(m_data.pos,SimpleMath::Vector2(1.0f,1.0f));

}

DrawBox::~DrawBox()
{
}

void DrawBox::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetNomalDeltaTime();

	// ŽžŠÔŒv‘ª
	if (m_hitMouseFlag || m_animationFlag)	m_animationData_First += deltaTime * 1.1f;
	else				m_animationData_First -= deltaTime * 1.1f;

	HitMouse();

	// ˜g‚ð“®‚©‚· ˆê’iŠK–Ú
	m_data.rage.x = Easing::EaseInOutCirc(m_data.rage.x * MAX_SIZE, m_saveData.rage.x,m_animationData_First);
	m_data.rage.y = Easing::EaseInOutCirc(m_data.rage.y * MAX_SIZE, m_saveData.rage.y,m_animationData_First);

	m_lineRage_Vertical.x = m_lineRage_Beside.y = Easing::EaseInOutCirc(MAX_SIZE, MIN_SIZE, m_animationData_First);

	m_lineRage_Vertical.y = m_data.rage.y * 2.0f;
	m_lineRage_Beside.x	  = m_data.rage.x * 2.0f;

	// “ñ’iŠK–Ú
	if (m_animationData_First) m_animationData_Second += deltaTime * 1.2f;
	if (m_animationData_Second.MaxCheck()) m_animationData_Second = 0.0f;

	m_color = SimpleMath::Color(1.0f, 0.0f, 1.0f - m_animationData_Second, 1.0f);

	m_keySelectFlag = false;

	m_clickPointer	->SetPos(m_data.pos + m_data.rage);
	m_clickPointer	->Update();

}

void DrawBox::Draw()
{

	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	SimpleMath::Vector2 popPos = SimpleMath::Vector2();
	RECT rect = RECT();

	pSB->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	{
		// ‰Ecü
		popPos = SimpleMath::Vector2(m_data.pos.x + m_data.rage.x, m_data.pos.y + m_data.rage.y);
		rect = RECT{ 2,2,1,1 };
		pSB->Draw(pSL.GetMissionLabelTexture().Get(),
			popPos,
			&rect,
			m_color,
			0.0f,
			SimpleMath::Vector2(0, 0),
			m_lineRage_Vertical);

		// ¶cü
		popPos = SimpleMath::Vector2(m_data.pos.x - m_data.rage.x, m_data.pos.y - m_data.rage.y);
		rect = RECT{ 1,1,2,2 };
		pSB->Draw(pSL.GetMissionLabelTexture().Get(),
			popPos,
			&rect,
			m_color,
			0.0f,
			SimpleMath::Vector2(0, 0),
			m_lineRage_Vertical);

		// ã‰¡ü
		popPos = SimpleMath::Vector2(m_data.pos.x + m_data.rage.x, m_data.pos.y - m_data.rage.y);
		rect = RECT{ 1,1,2,2 };
		pSB->Draw(pSL.GetMissionLabelTexture().Get(),
			popPos,
			&rect,
			m_color,
			0.0f,
			SimpleMath::Vector2(1, 0),
			m_lineRage_Beside);


		// ‰º‰¡ü
		popPos = SimpleMath::Vector2(m_data.pos.x - m_data.rage.x, m_data.pos.y + m_data.rage.y);
		rect = RECT{ 1,1,2,2 };
		pSB->Draw(pSL.GetMissionLabelTexture().Get(),
			popPos,
			&rect,
			m_color,
			0.0f,
			SimpleMath::Vector2(0, 0),
			m_lineRage_Beside);

		m_clickPointer->Draw();
	}
	pSB->End();

}

void DrawBox::SetPosRage(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage)
{

	m_data.pos = m_saveData.pos = pos;
	m_data.rage = m_saveData.rage = rage;

}
