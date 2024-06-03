#include "pch.h"
#include "PlayScene.h"

#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SoundData.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "NecromaLib/GameData/JsonLoder.h"

#include "Scenes/DataManager.h"

// 最大倍速量
#define MAX_SPEED 4

// スカイドームに適応する色
#define SKYDORM_LIGHTCOLOR SimpleMath::Color(0.2f, 0.5f, 3.3f, 1.0f)

PlayScene::PlayScene()
{
	m_stageNumber	= DataManager::GetInstance()->GetStageNum();
	m_maxWaveNum	= 0;

	ShareJsonData::GetInstance().LoadingJsonFile_Bullet();
	ShareJsonData::GetInstance().LoadingJsonFile_Machine();

	//　====================[　最大Wave数を取得する　]
	bool lastWaveCheck = false;
	while (!lastWaveCheck)
	{
		m_maxWaveNum++;
		ShareJsonData::GetInstance().LoadingJsonFile_Stage(m_stageNumber, m_maxWaveNum);
		lastWaveCheck = ShareJsonData::GetInstance().GetStageData().lastWave;

	}

	//　ステージナンバーのWave1を読み込む
	ShareJsonData::GetInstance().LoadingJsonFile_Stage(m_stageNumber, 1);

	// 等倍速に設定
	m_doubleSpeedNum = 1;

	m_uiTransparentTime = 0.0f;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	//　====================[　ポストプロセスの生成　]
	m_postProcess		= std::make_unique<MyPostProcess>();
	m_postProcess		->CreateShader();

	//　====================[　フィールドマネージャークラスの生成　]
	m_fieldManager		= std::make_unique<FieldObjectManager>();
	m_fieldManager		->Initialize();

	//　====================[　マウスポインタークラスの生成　] 
	m_mousePointer		= std::make_unique<MousePointer>();
	m_mousePointer		->Initialize();

	//　====================[　ユニット(マシン)マネージャークラスの生成　] 
	m_AM_Manager		= std::make_unique<AlchemicalMachineManager>();
	m_AM_Manager		->Initialize();

	//　====================[　カメラを動かすクラスの生成　]
	m_moveCamera		= std::make_unique<MoveCamera>();
	m_moveCamera		->Initialize();

	//　====================[　エネミーマネージャークラスの生成　]
	m_enemyManager		= std::make_unique<EnemyManager>();
	m_enemyManager		->Initialize();

	//　====================[　リソースを示すゲージクラスの生成　]
	m_resourceGauge		= std::make_unique<Gauge>();
	m_resourceGauge		->Initialize();

	//　====================[　ミッションマネージャークラスの生成　]
	m_missionManager	= std::make_unique<MissionManager>();
	m_missionManager	->Initialize(m_maxWaveNum);

	//　====================[　操作方法クラスの生成　]
	m_explanation		= std::make_unique<Explanation>();

	//　====================[　倍速ボタンの生成　]
	UI_Data uiData = ShareJsonData::GetInstance().GetUIData("OptionDouble");
	m_doubleSpeedButton = std::make_unique<SelectionBox>(uiData.pos, uiData.rage);

	UI_Data uiData_offset = ShareJsonData::GetInstance().GetUIData("OptionOffset");

	//　====================[　マウスが周辺に位置するかの判定処理を取得　]
	m_collider = std::make_unique<SelectionBox>(uiData.pos, SimpleMath::Vector2(uiData_offset.option["COLLIDER_X"], uiData_offset.option["COLLIDER_Y"]));
	m_collider->Initialize();
	m_collider->SetRect(RECT{ 0,0,1,1 });
	m_collider->SetWorldInfluenceFlag(false);

	//　====================[　天球モデルの読み込み　]
	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");
	m_skySphere = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/SkyDome.cmo", *fx);

	m_skySphere->UpdateEffects([&](IEffect* effect)
		{
			// ライトの取得
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// 当たる光の色を変更
			lights->SetAmbientLightColor(SKYDORM_LIGHTCOLOR);
		});

	//　====================[　用語説明クラスの生成　]
	m_operationInstructions = std::make_unique<OperationInstructions>();
	m_operationInstructions->Initialize(ShareJsonData::GetInstance().GetStageData().tutorial, this);

	//　====================[　チュートリアルクラスの生成　]
	m_tutorialManager = std::make_unique<TutorialManager>(this);
	m_tutorialManager->ChangeWave(1);

	//　====================[　レンダーテクスチャの作成　]
	m_offscreenRT	= std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
	m_offscreenRT	->SetDevice(pSD.GetDevice());
	RECT rect		= pSD.GetDeviceResources()->GetOutputSize();
	m_offscreenRT	->SetWindow(rect);
	m_offscreenRT	->GetShaderResourceView();

}

GAME_SCENE PlayScene::Update()
{
	ShareData& pSD = ShareData::GetInstance();
	SoundData& pSound = SoundData::GetInstance();
	DeltaTime& pDelta = DeltaTime::GetInstance();

	bool operationNow = m_operationInstructions->GetExplanationFlag();

	pSound.PlayBGM(XACT_WAVEBANK_BGMS_BGM_PLAY, false);

	//　====================[　倍速ボタンの更新　]
	m_doubleSpeedButton		->HitMouse();
	m_doubleSpeedNum		+= m_doubleSpeedButton->ClickMouse();
	//　　|=>　上限を超えたら等速から
	if (m_doubleSpeedNum > MAX_SPEED) m_doubleSpeedNum = 1;
	//　　|=>　倍速にする
	pDelta.SetDoubleSpeed((float)m_doubleSpeedNum);

	//　====================[　用語説明クラスの更新　]
	m_operationInstructions	->Update(this, m_moveCamera->GetStopCameraFlag());

	//　====================[　ミッションクラスの更新　]
	m_missionManager		->Update(m_AM_Manager.get(), m_enemyManager.get(), m_fieldManager.get());

	//　====================[　チュートリアルクラスの更新　]
	m_tutorialManager		->Update(m_missionManager->NextWaveFlag());

	//　====================[　マウス操作クラスの更新　]
	m_explanation			->Update();

	//　====================[　次のウェーブに進んだ際のリロード処理　]
	if (m_missionManager->NextWaveFlag())
	{
		DataManager* pDM = DataManager::GetInstance();
		ShareJsonData& pSJD = ShareJsonData::GetInstance();
		Stage_Data stageData = pSJD.GetStageData();

		//　　|=>　ウェーブ情報を再度取得する
		m_AM_Manager		->ReloadResource();
		m_enemyManager		->ReloadEnemyData();
		m_missionManager	->ReloadWave();
		m_tutorialManager	->ChangeWave(m_missionManager->GetWave());

		//　　|=>　リソース群を追加する
		pDM->SetNowBaseHP	(pDM->GetNowBaseHP()	+ stageData.resource.hp);
		pDM->SetNowCrystal	(pDM->GetNowCrystal()	+ stageData.resource.crystal);
		pDM->SetNowMP		(pDM->GetNowMP()		+ stageData.resource.mp);
		m_fieldManager->GetPlayerBase()->SetExp(m_fieldManager->GetPlayerBase()->GetExp() + stageData.resource.exp);

	}

	//　====================[　シーン遷移　]
	//　　|=>　タイトル
	if (m_operationInstructions->GetTitleSceneButton()->ClickMouse())	return GAME_SCENE::TITLE;
	//　　|=>　ステージセレクト
	if (m_operationInstructions->GetSelectSceneButton()->ClickMouse())	return GAME_SCENE::SELECT;

	//　====================[　チュートリアル中ならば以下の処理を行わない　]
	if (operationNow) 		return GAME_SCENE();

	//　====================[　カメラクラスの更新　]
	m_moveCamera		->Update(!m_AM_Manager->GetMachineSelect()->get()->GetHitMouseToSelectBoxEven(), true);

	//　====================[　フィールドマネージャークラスの更新　]
	m_fieldManager		->Update(m_enemyManager.get());
	
	//　====================[　マウスポインタークラスの更新　]
	m_mousePointer		->Update();

	//　====================[　ユニットマネージャーの更新　]
	m_AM_Manager		->Update(m_fieldManager.get(),
								 m_mousePointer.get(),
								 m_enemyManager.get(),
								 m_moveCamera.get());

	//　====================[　ミッションの時間更新　]
	m_missionManager	->TimerUpdate();

	//　====================[　エネミーマネージャークラスの更新　]
	m_enemyManager		->Update(m_fieldManager->GetPlayerBase()->GetPos());

	//　====================[　リソースゲージ描画クラスの更新　]
	m_resourceGauge		->Update();

	//　====================[　エネミーが本ウェーブ上に存在するかチェック　]
	bool enemyActivs = !m_enemyManager->GetEnemyData()->empty();

	//　====================[　エネミーToバレット　]
	if (!m_AM_Manager->GetBullet()->empty() && enemyActivs) EnemyToBullet();

	//　====================[　エネミーTo拠点　]
	if (enemyActivs)	  EnemyToPlayerBase();

	//　====================[　カメラクラスを元にカメラを動かす　]
	pSD.GetCamera()->SetViewMatrix(m_moveCamera->GetViewMatrix());
	pSD.GetCamera()->SetTargetPosition(m_moveCamera->GetTargetPosition());
	pSD.GetCamera()->SetEyePosition(m_moveCamera->GetEyePosition());

	//　====================[　敗北時　]
	if (m_missionManager->MissionFailure())		return GAME_SCENE::RESULT;

	//　====================[　ステージクリア時　]
	if (m_missionManager->MissionComplete())
	{
		// ステージ攻略情報を得る
		Json::WritingJsonFile_ClearData(m_stageNumber,
			m_AM_Manager->GetAlchemicalMachineObject(),
			m_missionManager->GetStartTimer());

		return GAME_SCENE::RESULT;
	}

	//　====================[　マシンのデータ再読み込み　]
	InputSupport* pINP = &InputSupport::GetInstance();
	if (pINP->GetKeybordState().IsKeyReleased(Keyboard::M))
	{
		ModelShader::GetInstance().CreateModelShader();
		ShareJsonData::GetInstance().LoadingJsonFile_Machine();
	}

	return GAME_SCENE();
}

void PlayScene::Draw()
{
	ShareData& pSD = ShareData::GetInstance();
	auto context = pSD.GetContext();

	ModelShader& pMS = ModelShader::GetInstance();

	// 深度マップ生成
	auto srv = pMS.GetShadowMap()->GetShaderResourceView();

	// 深度マップに書き込む
	DrawShadow();

	//　====================[　元に戻すためのレンダーターゲット　]
	auto renderTarget = pSD.GetDeviceResources()->GetRenderTargetView();
	auto depthStencil = pSD.GetDeviceResources()->GetDepthStencilView();

	//　====================[　新規レンダーターゲット　]
	auto offscreenRTV = m_offscreenRT->GetRenderTargetView();
	auto offscreenSRV = m_offscreenRT->GetShaderResourceView();

	// -------------------------------------------------------------------------- //
	// レンダーターゲットを変更（sceneRT）
	// -------------------------------------------------------------------------- //
	context->ClearRenderTargetView(m_offscreenRT->GetRenderTargetView(), Colors::White);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &offscreenRTV, depthStencil);
	// -------------------------------------------------------------------------- //

	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale({ 1.0f,1.0f,1.0f });
	modelData *= SimpleMath::Matrix::CreateTranslation({ 0.0f,10.0f,0.0f });

	// 天球描画
	m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
		{
		});

	//　====================[　フィールドオブジェクト描画　] 
	m_fieldManager->Draw();

	//　====================[　エネミー描画　]
	m_enemyManager->Render();

	//　====================[　マシンの描画　]
	m_AM_Manager->Render();

	pSD.GetSpriteBatch()->End();

	// -------------------------------------------------------------------------- //
	// レンダーターゲットとビューポートを元に戻す
	// -------------------------------------------------------------------------- //
	context->ClearRenderTargetView(renderTarget, Colors::White);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	auto const viewport = pSD.GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);
	// -------------------------------------------------------------------------- //

	// リソースの割り当てを解除する
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, nullsrv);

	// ポストプロセスを適応させる
	m_postProcess->Render(&offscreenSRV, &srv);

	// ==== 1パス目を描画する
	//pSD.GetSpriteBatch()->Begin();
	//pSD.GetSpriteBatch()->Draw(srv, SimpleMath::Vector2::Zero);
	//pSD.GetSpriteBatch()->End();

	// マウスポインターモデルを出す
	m_mousePointer->Draw();
	m_mousePointer->ModelDraw(m_AM_Manager->GetSelectModel());
}

void PlayScene::DrawShadow()
{
	ShareData& pSD		= ShareData::GetInstance();
	ModelShader& pMS	= ModelShader::GetInstance();
	auto device = ShareData::GetInstance().GetDeviceResources();
	auto context		= pSD.GetContext();

	//　====================[　新しいレンダーターゲット　]
	auto rtv = pMS.GetShadowMap()->GetRenderTargetView();
	auto dsv = pMS.GetDepthStencilView().Get();

	//　====================[　元に戻すためのレンダーターゲット　]
	auto renderTarget = device->GetRenderTargetView();
	auto depthStencil = device->GetDepthStencilView();

	//　====================[　レンダーターゲットを変更　]
	context->ClearRenderTargetView(pMS.GetShadowMap()->GetRenderTargetView(), Colors::Black);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &rtv, dsv);

	//　====================[　ビューポートを設定　]
	int width	= device->GetOutputSize().right;
	int height	= device->GetOutputSize().bottom;

	D3D11_VIEWPORT vp = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
	context->RSSetViewports(1, &vp);

	//　====================[　モデル描画　]
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale({ 1.0f,1.0f,1.0f });
	modelData *= SimpleMath::Matrix::CreateTranslation({ 0.0f,10.0f,0.0f });

	m_skySphere->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
		{
			ModelShader::GetInstance().ShadowModelDraw(false);
		});

	m_fieldManager	->WriteDepath();

	m_AM_Manager	->WriteDepath();

	// -------------------------------------------------------------------------- //
	// レンダーターゲットとビューポートを元に戻す
	// -------------------------------------------------------------------------- //
	context->ClearRenderTargetView(renderTarget, Colors::White);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	auto const viewport = device->GetScreenViewport();
	context->RSSetViewports(1, &viewport);
	// -------------------------------------------------------------------------- //
}

void PlayScene::DrawUI()
{
	ShareData& pSD	= ShareData::GetInstance();
	auto device		= ShareData::GetInstance().GetDeviceResources();
	float width		= static_cast<float>(device->GetOutputSize().right);
	float height	= static_cast<float>(device->GetOutputSize().bottom);

	SpriteLoder::TextureData bottomTexture = SpriteLoder::GetInstance().GetBottomLabel();

	pSD.GetSpriteBatch()->Begin();
	RECT rect = {0,0,bottomTexture.width,bottomTexture.height};
	pSD.GetSpriteBatch()->Draw(bottomTexture.tex.Get(),
		SimpleMath::Vector2(width / 2.0f, height),
		&rect,
		Colors::White,
		0.0f,
		SimpleMath::Vector2(bottomTexture.width / 2.0f, static_cast<float>(bottomTexture.height)));

	pSD.GetSpriteBatch()->End();

	//　====================[　UIの描画処理　]
	m_enemyManager			->RenderUI();
	m_AM_Manager			->DrawUI();

	m_fieldManager			->DrawBillboardUI();

	m_operationInstructions	->Render();

	m_tutorialManager		->Render();

	m_resourceGauge			->Render();

	//　　|=>　倍速アイコンから描画を行う
	m_doubleSpeedButton		->DrawUI(SpriteLoder::UIICON_TYPE::DISMATIONG + m_doubleSpeedNum);

	m_explanation			->Render(m_AM_Manager->GetMachineSelect()->get()->GetHitMouseToSelectBoxEven(), m_AM_Manager->GetRotateStopFlag());

	//　　|=>　描画順の関係により、最後に描画を行う
	m_operationInstructions	->Render_Layer2();
	m_tutorialManager		->Render_Layer2();

	m_missionManager->Render(m_maxWaveNum);
}

void PlayScene::Finalize()
{
	m_fieldManager->Finalize();
	m_fieldManager.reset();

	m_mousePointer->Finalize();
	m_mousePointer.reset();

	m_AM_Manager->Finalize();
	m_AM_Manager.reset();

	m_enemyManager->Finalize();
	m_enemyManager.reset();

	m_moveCamera.reset();

}

void PlayScene::EnemyToAMMachine()
{
}

void PlayScene::EnemyToPlayerBase()
{
	PlayerBase* playerBase = m_fieldManager->GetPlayerBase();

	for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
	{
		// ダウンキャストを行い、GameObject3D型に変換し判定の処理を得る
		bool hitEnemy = CircleCollider(playerBase->GetObject3D(), enemyIt->GetObject3D());

		// 当たり判定処理
		if (hitEnemy)
		{

			playerBase->Damage(enemyIt->GetPower());
			enemyIt->HitMachine(hitEnemy);
			enemyIt->SetStopFlag(hitEnemy);
			// 反発させる
			enemyIt->SetAccele(-enemyIt->GetAccele());

		}
	}
}

void PlayScene::EnemyToBullet()
{

	for (std::list<std::unique_ptr<Bullet>>::iterator bulletIt = m_AM_Manager->GetBullet()->begin(); bulletIt != m_AM_Manager->GetBullet()->end(); bulletIt++)
	{
		for (std::list<EnemyObject>::iterator enemyIt = m_enemyManager->GetEnemyData()->begin(); enemyIt != m_enemyManager->GetEnemyData()->end(); enemyIt++)
		{

			// 当たり判定を取るか否か
			if (!enemyIt->GetColliderActive()) continue;

			// 当たり判定処理
			if (CircleCollider(bulletIt->get()->GetObject3D(), enemyIt->GetObject3D()))
			{

				SimpleMath::Vector3 hitVectol = bulletIt->get()->GetPos() - enemyIt->GetPos();

				bulletIt->get()->SetEnemyHit(true);
				bulletIt->get()->SetLife(0);

				enemyIt->HitBullet(hitVectol, (int)bulletIt->get()->GetDamage());

			}
		}
	}
}

void PlayScene::TransparentUI(float transparentVal)
{
	m_doubleSpeedButton->SetColor(SimpleMath::Color(m_doubleSpeedButton->GetColorRGB(), transparentVal));

	SelectionBox* explanationButton = m_operationInstructions->GetExplanationButton();
	explanationButton->SetColor(SimpleMath::Color(explanationButton->GetColorRGB(), transparentVal));

}
