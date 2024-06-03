#include "pch.h"
#include "Explanation.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/InputSupport.h"

#include "NecromaLib/GameData/SpriteCutter.h"

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"

#define RATIO_X 1.2f

#define LETTER_ADJUSTMENT_POS_X 120

#define LETTER_POS_Y 660
#define LETTER_ADJUSTMENT_POS_Y 40

#define LETTER_TEX_W	128
#define LETTER_TEX_H	36

#define MOUSE_POS_Y 710.0f
#define MOUSE_COLOR 1.3f
#define MOUSE_ALPHA 0.7f
#define MOUSE_SIZE 0.235f

#define MOUSE_TEX_W	296
#define MOUSE_TEX_H	458

Explanation::Explanation():
	m_mouseSelectButton(),
	m_prvMouseValue()
{
}

Explanation::~Explanation()
{
}

void Explanation::Update()
{

	auto mouse = InputSupport::GetInstance().GetMouseState();
		
	m_mouseSelectButton[0] = InputSupport::GetInstance().LeftButton_Held();
	m_mouseSelectButton[2] = InputSupport::GetInstance().RightButton_Held();

	m_mouseSelectButton[1] = m_prvMouseValue != mouse.GetLastState().scrollWheelValue || 
							 InputSupport::GetInstance().MiddleButton_Held();

	m_prvMouseValue = mouse.GetLastState().scrollWheelValue;
}

void Explanation::Render(bool selectNowFlag, bool rotateStopFlag)
{

	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();

	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);

	pSB->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// É}ÉEÉXï`âÊ
	for (int i = 0; i < 3; i++)
	{

		RECT rect = SpriteCutter(MOUSE_TEX_W, MOUSE_TEX_H, i, 0);

		pSB->Draw(pSL.GetMouseTexture().Get(), SimpleMath::Vector2(width / RATIO_X, MOUSE_POS_Y),
			&rect,
			SimpleMath::Color(MOUSE_COLOR - m_mouseSelectButton[i], MOUSE_COLOR - m_mouseSelectButton[i], MOUSE_COLOR - m_mouseSelectButton[i], MOUSE_ALPHA),
			0.0f, SimpleMath::Vector2(MOUSE_TEX_W / 2, MOUSE_TEX_H / 2), MOUSE_SIZE);
	}

	ExplanationRender(RightExplanationPos(), MACHINE_SELECT + selectNowFlag);
	ExplanationRender(MiddleExplanationPos(), ZOOM + selectNowFlag);
	ExplanationRender(LeftExplanationPos(), ROTATE_STOP + rotateStopFlag);

	pSB->End();

}

SimpleMath::Vector2 Explanation::RightExplanationPos()
{
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);

	return SimpleMath::Vector2(width / RATIO_X - LETTER_ADJUSTMENT_POS_X, LETTER_POS_Y);
}

SimpleMath::Vector2 Explanation::MiddleExplanationPos()
{
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);

	return SimpleMath::Vector2(width / RATIO_X, LETTER_POS_Y - LETTER_ADJUSTMENT_POS_Y);
}

SimpleMath::Vector2 Explanation::LeftExplanationPos()
{
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);

	return SimpleMath::Vector2(width / RATIO_X + LETTER_ADJUSTMENT_POS_X, LETTER_POS_Y);
}

void Explanation::ExplanationRender(SimpleMath::Vector2 pos, int type)
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();

	RECT rect = SpriteCutter(LETTER_TEX_W, LETTER_TEX_H, type, 0);

	pSB->Draw(pSL.GetExplanationTextTexture().Get(), pos,
		&rect,
		SimpleMath::Color(Colors::White),
		0.0f,SimpleMath::Vector2(LETTER_TEX_W / 2, LETTER_TEX_H / 2),1.0f);

}
