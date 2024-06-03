#include "pch.h"
#include "SelectScene.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SoundData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "Scenes/Commons/DrawArrow.h"
#include "Scenes/Commons/UIKeyControl.h"

#include "Scenes/DataManager.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/PlayScene/UI/Number.h"

#define START_PROCESSING 0.35f	// 処理を開始するまでの時間

//　====================[　天球の情報　]
#define SKY_SCALE	0.8f
#define SKY_POS_Y	70.0f
#define SKY_LIGHT SimpleMath::Color(0.2f, 0.2f, 0.4f, 0.8f)

//　====================[　テクスチャの大きさ　]
#define TEXTURE_W 256
#define TEXTURE_H 64


SelectScene::SelectScene():
	m_selectStageNumber(1),
	m_changeMachine(true)
{
}

SelectScene::~SelectScene()
{
}

void SelectScene::Initialize()
{

	m_selectStageNumber = DataManager::GetInstance()->GetStageNum();

	ShareJsonData::GetInstance().LoadingJsonFile_ClearData(m_selectStageNumber);
	ShareJsonData::GetInstance().LoadingJsonFile_Stage(m_selectStageNumber,1);

	m_selectCamera = std::make_unique<SelectCamera>();
	m_selectCamera->Initialize();

	m_machineDraw = std::make_unique<DrawMachine>();
	m_machineDraw->Initialize(m_selectStageNumber);

	UI_Data ui_data = ShareJsonData::GetInstance().GetUIData("SelectMission");
	m_missionDraw = std::make_unique<MissionRender>(ui_data.pos, ui_data.rage);

	ui_data = ShareJsonData::GetInstance().GetUIData("SelectLeft");
	m_arrowDraw[0] = std::make_unique<DrawArrow>(ui_data.pos, ui_data.rage,0);
	ui_data = ShareJsonData::GetInstance().GetUIData("SelectRight");
	m_arrowDraw[1] = std::make_unique<DrawArrow>(ui_data.pos, ui_data.rage,2);
	ui_data = ShareJsonData::GetInstance().GetUIData("SelectMiddle");
	m_nextSceneBox = std::make_unique<SelectionBox>(ui_data.pos, ui_data.rage);

	ui_data = ShareJsonData::GetInstance().GetUIData("SelectNumber");
	m_stageNumber = std::make_unique<Number>(ui_data.pos, ui_data.rage);
	m_stageNumber->SetNumber(m_selectStageNumber);

	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_skySphere = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/SkySphere.cmo", *fx);

	m_skySphere->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// 光の当たり方変更
			lights->SetAmbientLightColor(SKY_LIGHT);

		});

}

GAME_SCENE SelectScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	SoundData& pSound = SoundData::GetInstance();

	pSound.PlayBGM(XACT_WAVEBANK_BGMS_BGM_SELECT, false);

	m_selectCamera->Update();
	m_machineDraw->Update();

	//　====================[　カメラを動かす　]
	pSD.GetCamera()->SetViewMatrix(m_selectCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition(m_selectCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition(m_selectCamera->GetEyePosition());
 
	//　====================[　UI更新処理　]
	//　　|=>　右側矢印
	m_arrowDraw[0]->HitMouse();
	m_arrowDraw[0]->SetActiveFlag(m_selectStageNumber > 1);
	if (m_arrowDraw[0]->ClickMouse())
	{
		m_selectStageNumber--;
		m_changeMachine = false;
		m_selectCamera->AnimationReset();
	}
	//　　|=>　左側矢印
	m_arrowDraw[1]->HitMouse();
	m_arrowDraw[1]->SetActiveFlag(m_selectStageNumber < ShareJsonData::GetInstance().GetGameParameter().stage_Max);

	if (m_arrowDraw[1]->ClickMouse())
	{
		m_selectStageNumber++;
		m_changeMachine = false;
		m_selectCamera->AnimationReset();
	}

	//　====================[　ステージ情報読み込み　]
	if (m_selectCamera->GetAnimationTimer() >= START_PROCESSING && !m_changeMachine)
	{
		// 読み込み
		ShareJsonData::GetInstance().LoadingJsonFile_ClearData(m_selectStageNumber);
		ShareJsonData::GetInstance().LoadingJsonFile_Stage(m_selectStageNumber,1);

		m_machineDraw->Initialize(m_selectStageNumber);
		m_stageNumber->SetNumber(m_selectStageNumber);
		m_changeMachine = true;
	}

	m_nextSceneBox->HitMouse();

	//　プレイシーンに遷移 (ボタンを押した時)
	if (m_nextSceneBox->ClickMouse())
	{
		DataManager::GetInstance()->SetStageNum(m_selectStageNumber);

		return GAME_SCENE::PLAY;
	}

	return GAME_SCENE();
}

void SelectScene::Draw()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale({ SKY_SCALE });
	modelData = SimpleMath::Matrix::CreateTranslation({ 0.0f,SKY_POS_Y,0.0f });

	// 天球描画
	m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
		{

			ModelShader::GetInstance().ModelDrawShader(
				SimpleMath::Color(Colors::White),
				SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SpriteLoder::GetInstance().GetMachineTextuer(2));

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetMachineTextuer(2).GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetNormalMap(2).GetAddressOf());

			//　====================[　深度ステンシルステートの設定　]
			pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthDefault(), 0);

		});


	pSB->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	m_machineDraw->Render();
	m_missionDraw->Render(ShareJsonData::GetInstance().GetStageData());
	
	pSB->End();

}

void SelectScene::DrawUI()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();


	m_nextSceneBox->Draw();

	m_stageNumber->SetColor(SimpleMath::Color(Colors::White));
	m_stageNumber->Render();

	if (m_arrowDraw[0]->GetActiveFlag()) m_arrowDraw[0]->Draw();
	if (m_arrowDraw[1]->GetActiveFlag()) m_arrowDraw[1]->Draw();

	pSB->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());


	RECT rect = { 0,0,TEXTURE_W,TEXTURE_H };

	pSB->Draw(SpriteLoder::GetInstance().GetGameStartTexture().Get(), m_nextSceneBox->GetPos(),&rect,Colors::White,0.0f,SimpleMath::Vector2((float)rect.right / 2.0f, (float)rect.bottom / 2.0f));

	pSB->End();

}

void SelectScene::Finalize()
{
}
