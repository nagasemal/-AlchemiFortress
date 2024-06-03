#include "pch.h"
#include "FieldObjectManager.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

#include "Scenes/PlayScene/Enemy/EnemyManager.h"

#include <algorithm>
#include <random>

#include "DirectXHelpers.h"

// ワールド空間に出すUIの大きさ調整
#define WORLD_UI_SIZE 0.01f

FieldObjectManager::FieldObjectManager():
	m_crystalTomouseFlag()
{
}

FieldObjectManager::~FieldObjectManager()
{
}

void FieldObjectManager::Initialize()
{
	ShareData& pSD = ShareData::GetInstance();

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	// モデルの読み込み
	m_floorModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Ground.cmo", *fx);
	m_crystalModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Crystal.cmo", *fx);
	m_baseModel_Towor = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Tower_1.cmo", *fx);
	m_baseModel_Pillar = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Tower_2.cmo", *fx);

	// 結晶モデルのエフェクトの設定
	m_crystalModel->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// 光の当たり方変更
			lights->SetAmbientLightColor(SimpleMath::Color(0.4f, 0.4f, 1.f, 0.2f));

		});

	// エフェクトの設定
	m_baseModel_Towor->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);

			// 色変更
			lights->SetLightDiffuseColor(0, Colors::Gray);
			lights->SetLightDiffuseColor(1, Colors::Gray);
			lights->SetLightDiffuseColor(2, Colors::Gray);

		});

	// 生成
	m_field = std::make_unique<Field>();
	m_playerBase = std::make_unique<PlayerBase>();
	m_crystals = std::make_unique<std::list<Crystal>>();

	m_field->Initialize();
	m_playerBase->Initialize();

	CrystalSpawn(30);

	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	for (int i = 0; i < pSJD.GetStageData().crystalPos.size(); i++)
	{
		SimpleMath::Vector2 loadCrysralPos = pSJD.GetStageData().crystalPos[i];

		SimpleMath::Vector3 crystalPos = SimpleMath::Vector3(loadCrysralPos.x,-1.0f, loadCrysralPos.y);

		m_crystals->push_back(*std::make_unique<Crystal>(crystalPos, (float)cos(5)));
	}

	// クリスタルの初期化処理
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->Initialize();
	}

	//　====================[　ビルボード描画に使用　]
//　　|=> エフェクトを作成
	m_effect = std::make_unique<BasicEffect>(ShareData::GetInstance().GetDevice());
	m_effect->SetTextureEnabled(true);
	m_effect->SetVertexColorEnabled(true);
	m_effect->SetLightingEnabled(false);

	//　　|=> 入力レイアウトを作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect(
			ShareData::GetInstance().GetDevice(),
			m_effect.get(),
			VertexPositionColorTexture::InputElements,
			VertexPositionColorTexture::InputElementCount,
			m_inputLayout.ReleaseAndGetAddressOf())
	);

}

void FieldObjectManager::Update(EnemyManager* pEnemyManager)
{

	// 床の更新処理
	m_field->Update();

	// プレイヤー拠点の更新処理
	m_playerBase->Update();

	m_crystalTomouseFlag = false;

	m_playerBase->SetExp(pEnemyManager->GetNockDownEnemyExp() + m_playerBase->GetExp());

	// クリスタルの更新処理
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->Update();

		// クリスタルがマウスに触れている
		if (!m_crystalTomouseFlag)
		{
			m_crystalTomouseFlag = it->GetHitMouse();
		}
	}

}

void FieldObjectManager::Draw()
{

	// 床の描画処理
	m_field->Render(m_floorModel.get());

	// プレイヤー拠点の描画処理
	m_playerBase->Render(m_baseModel_Towor.get());

	// クリスタルの描画処理
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->Render(m_crystalModel.get());
	}

}

void FieldObjectManager::DrawBillboardUI()
{
	ShareData& pSD = ShareData::GetInstance();
	auto status = pSD.GetCommonStates();
	auto camera = ShareData::GetInstance().GetCamera();
	auto context = pSD.GetContext();

	//　====================[　ビルボード行列生成　]
	//　　|=>　スクリーン座標はY軸が＋－逆なので-1をかける
	SimpleMath::Matrix invertY = SimpleMath::Matrix::CreateScale(1.0f, -1.0f, 1.0f);

	//　　|=> ビュー行列の回転を打ち消す行列を作成する
	SimpleMath::Matrix invView = camera->GetViewMatrix().Invert();
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	//　　|=> エフェクトにビュー行列と射影行列を設定する
	m_effect->SetView(camera->GetViewMatrix());
	m_effect->SetProjection(camera->GetProjectionMatrix());


	//　====================[　UIをワールド空間に出す]
	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, status->NonPremultiplied(), nullptr, status->DepthNone(), status->CullCounterClockwise(), [=]
		{
			// ワールド行列作成
			SimpleMath::Matrix world =
				SimpleMath::Matrix::CreateScale(WORLD_UI_SIZE) *
				invertY *
				invView *
				SimpleMath::Matrix::CreateTranslation(m_playerBase->GetPos() + SimpleMath::Vector3(0.0f, 8.0f, 0.0f));

			// エフェクトを適応する
			m_effect->SetWorld(world);
			m_effect->Apply(context);
			// 入力レイアウトを設定する
			context->IASetInputLayout(m_inputLayout.Get());
		});

	m_playerBase->RenderBillboardUI();

	pSD.GetSpriteBatch()->End();

}

void FieldObjectManager::WriteDepath()
{
	// 床の描画処理
	m_field->WriteDepth(m_floorModel.get());

	// プレイヤー拠点の描画処理
	m_playerBase->WriteDepth(m_baseModel_Towor.get());

	// クリスタルの描画処理
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->WriteDepth(m_crystalModel.get());
	}


}

void FieldObjectManager::Finalize()
{
	// 床の削除処理
	m_field->Finalize();
	m_field.reset();

	// プレイヤー拠点の更新処理
	m_playerBase->Finalize();
	m_playerBase.reset();

	// クリスタルの描画処理
	for (std::list<Crystal>::iterator it = m_crystals->begin(); it != m_crystals->end(); it++)
	{
		it->Finalize();
	}

	m_crystals.reset();

	delete m_crystals.get();

}

void FieldObjectManager::CrystalSpawn(int num)
{
	num;
}
