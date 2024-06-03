#include "pch.h"
#include "OperationInstructions.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/InputSupport.h"

#include "NecromaLib/GameData/UserInterfase.h"
#include "NecromaLib/GameData/SpriteCutter.h"

#include "Scenes/PlayScene/UI/SelectionBox.h"

#include "Scenes/Commons/DrawArrow.h"
#include "Scenes/Commons/DrawVariableNum.h"
#include "Scenes/Commons/DrawLine.h"
#include "Scenes/Commons/DrawBox.h"

#include "NecromaLib/GameData/ScreenToWorld.h"

// UIを指すにあたり必要となるクラス
#include "Scenes/PlayScene/PlayScene.h"
#include "Scenes/PlayScene/AlchemicalMachine/AM_Attacker.h"
#include "Scenes/SelectScene/MissionRender.h"
#include "Scenes/PlayScene/Shadow/Particle.h"

// 示すラインの大きさ
#define LINE_RAGE SimpleMath::Vector2(50.0f,50.0f)

#define TEX_W 128
#define TEX_H 28

const std::vector<const wchar_t*> OperationInstructions::FILENAME =
{
	{L"None.png"},			//  0.None
	{L"Attacker.png"},		//  1.Attacker
	{L"Defencer.png"},		//  2.Defencer
	{L"Upper.png"},			//  3.Upper
	{L"Recovery.png"},		//  4.Recovery
	{L"Excavator.png"},		//  5.Excavator
	{L"RotateStop.png"},	//  6.RotateStop
	{L"Gauge_1.png"},		//  7.HPゲージ
	{L"Gauge_2.png"},		//  8.MPゲージ
	{L"Gauge_3.png"},		//  9.クリスタルゲージ
	{L"Alchemical.png"},	// 10.錬金ボタン
	{L"CameraMove.png"},	// 11.カメラ移動
	{L"Tips1.png"},			// 12.ユニット耐久値について
	{L"Tips2.png"},			// 13.回転ストップ活用例
	{L"Tips3.png"},			// 14.選択中のユニットを変えるには
	{L"Tips4.png"}			// 15.ゲームスピード
};

OperationInstructions::OperationInstructions():
	m_explanationFlag(false),
	m_cameraFlag(false),
	m_selectNumber(0),
	m_maxSelectVal(),
	m_tutorialTime(0)
{

}

OperationInstructions::~OperationInstructions()
{
}

void OperationInstructions::Initialize(std::vector<Tutorial_Status> tutorialNumber, PlayScene* pPlayScene)
{
	pPlayScene;
	// 取得
	UI_Data uiData = ShareJsonData::GetInstance().GetUIData("OptionExplanation");
	m_explanationButton = std::make_unique<SelectionBox>(uiData.pos, uiData.rage);

	uiData = ShareJsonData::GetInstance().GetUIData("OperationArrowL");
	m_arrowL = std::make_unique<DrawArrow>(uiData.pos, uiData.rage,4);

	uiData = ShareJsonData::GetInstance().GetUIData("OperationArrowR");
	m_arrowR = std::make_unique<DrawArrow>(uiData.pos, uiData.rage,2);

	uiData = ShareJsonData::GetInstance().GetUIData("OperationTitle");
	m_titleSceneBox = std::make_unique<SelectionBox>(uiData.pos, uiData.rage);

	uiData = ShareJsonData::GetInstance().GetUIData("OperationSelect");
	m_selectSceneBox = std::make_unique<SelectionBox>(uiData.pos, uiData.rage);

	CreateInterfase();

	// 説明画像テクスチャの読み込み
	m_textTexture->LoadTexture(FileNamePath(FILENAME[0]));

}

void OperationInstructions::Update(PlayScene* pPlayScene, bool stopFlag)
{
	pPlayScene;
	m_cameraFlag = stopFlag;

	// 説明モード移行ボタン
	m_explanationButton->HitMouse();

	// 選択モード解除/起動
	if (m_explanationButton->ClickMouse())
	{
		m_explanationFlag = !m_explanationFlag;
	}


	// 説明時モード
	if (!m_explanationFlag) return;
	
	m_arrowL->HitMouse();
	m_arrowR->HitMouse();
	
	m_titleSceneBox->HitMouse();
	m_selectSceneBox->HitMouse();
	
	// 左ボタンでm_selectNumber増加
	if (m_arrowL->ClickMouse())
	{
		m_selectNumber--;
		// 上限下限設定
		m_selectNumber = std::min(std::max(m_selectNumber, 0), (const int)FILENAME.size() - 1);
		m_textTexture->LoadTexture(FileNamePath(FILENAME[m_selectNumber]));
	}
	
	// 右ボタンでm_selectNumber増加
	if (m_arrowR->ClickMouse())
	{
		m_selectNumber++;
		// 上限下限設定
		m_selectNumber = std::min(std::max(m_selectNumber, 0), (const int)FILENAME.size() - 1);
		m_textTexture->LoadTexture(FileNamePath(FILENAME[m_selectNumber]));
	}

}

void OperationInstructions::Render()
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	SpriteBatch* pSB = pSD.GetSpriteBatch();

	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = (float)device->GetOutputSize().right;
	float height = (float)device->GetOutputSize().bottom;

	if (m_explanationFlag)
	{

		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	
		RECT rect = { 0,0,800,600};

		// 背景を暗くする
		pSB->Draw(pSL.GetMagicRule(0).Get(), SimpleMath::Vector2(width / 2.0f,height / 2.0f),
				  &rect,
				  SimpleMath::Color(0.0f,0.0f,0.0f,0.3f),
				  0.0f,
				  DirectX::XMFLOAT2{ rect.right / 2.0f,rect.bottom / 2.0f}, 2.0f);
	
	
		pSD.GetSpriteBatch()->End();

		m_titleSceneBox		->DrawUI(pSL.GetResultTextTexture().Get(), SpriteCutter(TEX_W, TEX_H, 3, 0), SimpleMath::Color(Colors::Black),1.0f, TEX_W, TEX_H);
		m_selectSceneBox	->DrawUI(pSL.GetResultTextTexture().Get(), SpriteCutter(TEX_W, TEX_H, 1, 0), SimpleMath::Color(Colors::Black),1.0f, TEX_W, TEX_H);

		// 閉じるボタン描画
		m_explanationButton->DrawUI(SpriteLoder::UIICON_TYPE::CANCEL);
	}
	else
	{
		// 説明モードボタン描画
		m_explanationButton->DrawUI(SpriteLoder::UIICON_TYPE::EXPRANATION);
	}
}

void OperationInstructions::Render_Layer2()
{	
	if ( m_explanationFlag )
	{
		// テキスト描画
		m_backFlame		->Render();
		m_textTexture	->Render();
	
		// 選択移動矢印の描画　(上限下限に達したら描画を切る)
		if (m_selectNumber < (const int)FILENAME.size() - 1)	m_arrowR->Draw();
		if (m_selectNumber >= 1)								m_arrowL->Draw();

	}
}

void OperationInstructions::Finalize()
{
}

void OperationInstructions::CreateInterfase()
{
	auto device = ShareData::GetInstance().GetDeviceResources();

	// テクスチャを描画する際の位置情報と大きさ
	UI_Data uiData = ShareJsonData::GetInstance().GetUIData("OperationText");

	m_textTexture	= std::make_unique<UserInterface>();
	m_backFlame		= std::make_unique<UserInterface>();

	m_textTexture->Create(device, L"Resources/Textures/Explanation/Attacker.png", uiData.pos, uiData.rage,UserInterface::MIDDLE_CENTER);

	m_backFlame->Create(device, L"Resources/Textures/Explanation/BackFlame.png", uiData.pos,SimpleMath::Vector2(uiData.option["BACK_RAGE"]), UserInterface::MIDDLE_CENTER);
	m_backFlame->SetColor(SimpleMath::Color(0.8f, 0.8f, 1.0f, 0.95f));
}

std::wstring OperationInstructions::FileNamePath(const wchar_t* path)
{

	std::wstring pathName = L"Resources/Textures/Explanation/";

	pathName += path;

	return pathName;
}
