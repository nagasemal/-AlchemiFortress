#include "pch.h"
#include "TitleScene.h"

#include "Scenes/PlayScene/Shadow/MagicCircle.h"
#include "Scenes/Commons/DrawLine.h"
#include "Scenes/Commons/DrawBox.h"

#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SoundData.h"

#define ALPHA_VAL 0.55f			// 右上魔法陣の透明度
#define ROTATE_VAL 0.5f			// 右上魔法陣の回転角度
#define SCALE_VAL 0.5f			// 右上魔法陣の大きさ

#define MAGIC_CIRCLE_RAGE 30	// 3D空間上の魔法陣の大きさ

#define START_PROCESSING 0.75f	// 処理を開始するまでの時間

#define SKY_SPHERE_SCALE 2.0f	// 天球モデルの大きさ
#define SKY_SPHERE_POS_Y 70.0f	// 天球モデルの位置
#define SKY_SPHERE_LIGHTCOLOR	SimpleMath::Color(0.5f, 0.7f, 1.f, 0.8f)	// 天球モデルのアンビエントカラー
#define SKY_SPHERE_ROTATION		SimpleMath::Vector3(20.0f, 15.0f, 0.0f)		// 天球モデルの回転角度

#define VEIL_COLOR SimpleMath::Color(0.4f, 0.4f, 0.4f, 0.5f)	// 幕の色

TitleScene::TitleScene():
	m_nextType(ButtonType::Num),
	m_rotateNowFlag(),
	m_rotateYSpeed()
{
	ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	ShareJsonData::GetInstance().LoadingJsonFile_GameParameter();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	SpriteLoder& pSL = SpriteLoder::GetInstance();

	//　====================[　カメラの生成　]
	m_titleCamera = std::make_unique<TitleCamera>();
	m_titleCamera->Initialize();

	//　====================[　魔法陣描画クラスの生成　]
	m_magicCircle = std::make_unique<MagicCircle>();
	m_magicCircle->Initialize();

	//　====================[　天球モデルを生成　]
	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_skySphere = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/SkySphere.cmo", *fx);

	//　　|=>　エフェクトをかける
	m_skySphere->UpdateEffects([&](IEffect* effect)
		{
			// ライトの取得
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// 当たる光の色を変更
			lights->SetAmbientLightColor(SKY_SPHERE_LIGHTCOLOR);

		});

	//　====================[　タイトルロゴの設定　]
	UI_Data ui_data = ShareJsonData::GetInstance().GetUIData("TitleLogo");
	m_titleLogo = std::make_unique<TitleLogo>();
	m_titleLogo->Create(pSL.GetTitleLogoPath());
	m_titleLogo->SetWindowSize(width, height);
	m_titleLogo->SetColor(SimpleMath::Color(Colors::BlueViolet));
	m_titleLogo->SetPosition(ui_data.pos);

	//　====================[　画面右側の幕の設定　]
	ui_data = ShareJsonData::GetInstance().GetUIData("TitleVeil");
	m_veil = std::make_unique<Veil>(0);
	m_veil->Create(L"Resources/Textures/TitleText.png");
	m_veil->LoadShaderFile(L"Veil");
	m_veil->SetWindowSize(width, height);
	m_veil->SetColor(VEIL_COLOR);
	m_veil->SetScale(ui_data.rage);
	m_veil->SetPosition(ui_data.pos);

	//　====================[　キーボード上でUIを操作するクラスの生成　]
	m_uiKeyControl = std::make_unique<UIKeyControl>();

	//　====================[　ゲーム内設定を決めるクラスの生成　]
	m_renderOption = std::make_unique<RenderOption>();


	//　====================[　UIを生成,登録　]
	ui_data = ShareJsonData::GetInstance().GetUIData("TitleButton");
	for (int i = 0; i < ButtonType::Num; i++)
	{
		m_selectionButton[i] = std::make_unique<DrawLine>(SimpleMath::Vector2(ui_data.pos.x, ui_data.pos.y + (i * ui_data.option["SHIFT_Y"])),ui_data.rage);

		m_uiKeyControl->AddUI(m_selectionButton[i].get());
	}

	//　====================[　マシン描画クラスの生成　]
	m_titleCall = std::make_unique<DrawMachine>();
	m_titleCall->Initialize();

	//　====================[　変数の初期化　]
	m_animationData = AnimationData();
	m_rotateNowFlag = false;

}

GAME_SCENE TitleScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	InputSupport* pINP = &InputSupport::GetInstance();
	SoundData& pSound = SoundData::GetInstance();

	//　====================[　BGMを再生　]
	pSound.PlayBGM(XACT_WAVEBANK_BGMS_BGM_TITLE, false);

	//　====================[　タイトルロゴの表示　]
	m_titleCall->Update();

	//　====================[　カメラの更新　]
	m_titleCamera->Update();
	
	//　====================[　魔法陣の更新　]
	m_magicCircle->Update();

	//　====================[　一定時間経過で処理の追加　]
	if (m_titleCamera->GetAnimTime() >= START_PROCESSING)
	{
		// タイトルロゴの更新
		m_titleLogo->Update();

		// 右側から現れる黒幕の更新
		m_veil->Update();


		// ボタンのアップデート
		for (int i = 0; i < ButtonType::Num; i++)
		{
			m_selectionButton[i]->Update();
		}

		// UIのキーボード操作クラスの更新
		m_uiKeyControl->Update();
	}

	//　====================[　カメラを動かす　]
	pSD.GetCamera()->SetViewMatrix(m_titleCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition(m_titleCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition(m_titleCamera->GetEyePosition());

	//　====================[　魔法陣展開　]
	m_magicCircle->CreateMagicCircle(SimpleMath::Vector3::Zero, MAGIC_CIRCLE_RAGE);

	//　====================[　左クリックをした際の処理　]
	//　　|=>　変数のリセット
	if (pINP->LeftButton_Press() && !m_rotateNowFlag)
	{
		m_animationData = 0.0f;
		m_rotateNowFlag = true;
	}

	//　====================[　マシンを動かす　]
	if (m_rotateNowFlag)
	{
		m_animationData += DeltaTime::GetInstance().GetDeltaTime();
		m_animationData.anim = Easing::EaseInOutCubic(0.0f, XMConvertToRadians(90.0f), m_animationData);

		// 右上の魔法陣の回転が終わったらシーン遷移を開始するため
		// アニメーション変数が最大値に至っていなければTrueとする
		m_rotateNowFlag = !m_animationData.MaxCheck();

	}

	//　====================[　オプション操作を辞める　]
	if (m_renderOption->GetHitCancelButton())
	{
		m_nextType = ButtonType::Num;
	}

	//　====================[　選択されている項目がオプションである]
	if (m_nextType == ButtonType::Option)
	{
		m_renderOption->Update();
	}
	//　====================[　選択されている項目がオプションでない　]
	else
	{
		// 各項目(はじめから,つづきから,オプション,ゲーム終了)の更新処理
		for (int i = 0; i < ButtonType::Num; i++)
		{
			if (m_selectionButton[i]->ClickMouse()) m_nextType = (ButtonType)i;
		}
	}

	//　====================[　シーン遷移を行わない　]
	if (m_rotateNowFlag) 	return GAME_SCENE();

	//　====================[　ステージセレクトシーンに遷移　]
	//　　|=>　初めから
	if (m_nextType == ButtonType::NewGame)
	{
		Json::InitializationClearStageData();

		return GAME_SCENE::PLAY;
	}

	//　====================[　ステージセレクトシーンに遷移　]
	//　　|=>　続きから
	if (m_nextType == ButtonType::Restert) 	return GAME_SCENE::SELECT;

	//　====================[　ゲームを終了する　]
	if (m_nextType == ButtonType::Exit)		PostQuitMessage(0);

	return GAME_SCENE();
}

void TitleScene::Draw()
{
	ShareData& pSD = ShareData::GetInstance();

	//　====================[　天球の行列設定　]
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateTranslation({ 0.0f,SKY_SPHERE_POS_Y,0.0f });
	modelData *= SimpleMath::Matrix::CreateFromYawPitchRoll(SKY_SPHERE_ROTATION);
	modelData *= SimpleMath::Matrix::CreateScale(SKY_SPHERE_SCALE);

	//　====================[　天球の描画　]
	m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection());

	//　====================[　マシンの描画　]
	m_titleCall->Render();

	//　====================[　魔法陣の描画　]
	m_magicCircle->CreateWorld();
	m_magicCircle->Render(0);
}

void TitleScene::DrawUI()
{
	// 必要なリソースの確保
	ShareData& pSD		= ShareData::GetInstance();
	SpriteLoder::TextureData texData = SpriteLoder::GetInstance().GetMachineMagicCircleTexture(1);
	auto pSB			= pSD.GetSpriteBatch();
	auto device			= pSD.GetDeviceResources();

	int width			= device->GetOutputSize().right;
	int height			= device->GetOutputSize().bottom;

	height;

	//　====================[　半透明帯の描画　]
	m_veil->Render();

	//　====================[　UI描画の開始　]
	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	//　　|=> 画像のサイズ
	RECT rect_circle	= { 0, 0, texData.width, texData.height};

	//　　|=> 画像の位置設定
	SimpleMath::Vector2 box_Pos = { (float)width,0.0f};

	//　　|=>  右上にクリック時回転する魔法陣を描画
	pSB->Draw(texData.tex.Get(),
			  box_Pos,
			  &rect_circle,
			  SimpleMath::Color(1.0f,1.0f,1.0f, ALPHA_VAL),
			  m_animationData.anim,
			  DirectX::XMFLOAT2(static_cast<float>(texData.width / 2), static_cast<float>(texData.height / 2)),
			  SCALE_VAL);

	//　　|=>  ボタンの描画
	for (int i = 0; i < ButtonType::Num; i++)
	{
		m_selectionButton[i]->Draw();
	}

	pSB->End();

	//　====================[　ゲームロゴの描画　]
	m_titleLogo->Render();

	//　====================[　オプション画面の描画　]
	if (m_nextType == ButtonType::Option)
	{
		m_renderOption->Render();
	}
}

void TitleScene::Finalize()
{
}
