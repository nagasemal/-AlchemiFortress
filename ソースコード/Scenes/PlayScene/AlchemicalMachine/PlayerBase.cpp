#include "pch.h"
#include "PlayerBase.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/ModelShader.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/DataManager.h"

#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/GameData/SpriteCutter.h"

// 塔の大きさ
#define RAGE SimpleMath::Vector3(3, 3, 3)

//　====================[　3DUIにて使用する数値の定義　]
#define REDUCE 0.05f

//　　|=> HPゲージの大きさ
#define HPGAUGE_RAGE		0.7f

//　　|=> マシンの名前の大きさ
#define MACHINENAME_RAGE	2.35f

//　　|=> Lv表示の大きさ
#define MACHINELV_RAGE		2.0f

//　　|=> マシンのアイコンの大きさ
#define MACHINEICON_RAGE	0.8f

//　　|=> 魔法陣の大きさ
#define MAGICCIRCLE_RAGE	0.2f

//　　|=> UIの透明度
#define UI_ALPHAVAL			0.5f

//　　|=>　UIが上方へ登る際の値
#define UI_UPVAL			50.0f

//　　|=>  数字画像の縦横大きさ
#define SPRITE_RAGE			64.0f


PlayerBase::PlayerBase() :
	m_baseLv(1),
	m_hp(1),
	m_exp(),
	m_testBox(),
	m_hitMouseFlag(),
	m_lvUpTiming(),
	m_invincibleTime(),
	m_difRedioHp()
{
}

PlayerBase::~PlayerBase()
{
} 

void PlayerBase::Initialize()
{
	m_baseLv = ShareJsonData::GetInstance().GetStageData().resource.lv;
	m_exp = 0;
	m_hp = (float)DataManager::GetInstance()->GetNowBaseHP();

	m_data.pos  = SimpleMath::Vector3::Zero;
	m_data.rage = RAGE;

	auto pDataM = DataManager::GetInstance();

	pDataM->MPMAXRecalculation		(m_baseLv);
	pDataM->CrystalMAXRecalculation	(m_baseLv);
	pDataM->BaseHPMAXRecalculation	(m_baseLv);

	pDataM->Initialize();

	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");
	m_baseModel_Towor = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Tower_1.cmo", *fx);
	m_baseModel_Pillar = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Tower_2.cmo", *fx);

}

void PlayerBase::Update()
{
	InputSupport& pINP = InputSupport::GetInstance();

	DataManager& pDataM = *DataManager::GetInstance();

	int enemyKillNum = 0;
		
	enemyKillNum = DataManager::GetInstance()->GetNowEnemyKill();

	// 仮置き
	auto mouse = pINP.GetMouseState();
	bool rightRelease = pINP.RightButton_Release();
	rightRelease;

	m_hitMouseFlag = false;
	m_lvUpTiming = false;

	// 無敵時間の計測
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();
	m_invincibleTime += deltaTime;

	SimpleMath::Vector3 mouseWolrdPos = InputSupport::GetInstance().GetMousePosWolrd();

	if (PointerToCircle(GetCircle(), mouseWolrdPos)) m_hitMouseFlag = true;

	if (m_exp >= GetNextLvEXP() && m_baseLv < ShareJsonData::GetInstance().GetGameParameter().baseLV_MAX)
	{
		// 超過分は繰越す
		float saveEXP = GetNextLvEXP() - m_exp;
		m_baseLv++;
		m_lvUpTiming = true;

		// 最大魔力量　最大結晶量　最大HP量 再計算
		pDataM.MPMAXRecalculation(m_baseLv);
		pDataM.CrystalMAXRecalculation(m_baseLv);
		pDataM.BaseHPMAXRecalculation(m_baseLv);

		// 超過分を入れる
		m_exp = saveEXP;

	}

	//　====================[　HPゲージの処理　]
	//　　|=>　割合計算
	float radio_Hp    = (static_cast<float>(pDataM.GetNowBaseHP()) / static_cast<float>(pDataM.GetNowBaseHP_MAX()));
	//　　|=> 徐々に減るHPの処理
	m_difRedioHp      -= REDUCE * deltaTime;
	m_difRedioHp      = std::min(std::max(m_difRedioHp, radio_Hp), 1.0f);
}

void PlayerBase::Draw()
{
	ShareData& pSD = ShareData::GetInstance();

	SimpleMath::Matrix textBox = SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y + 3.0f, m_data.pos.z);

	m_testBox->Draw(textBox, pSD.GetView(), pSD.GetProjection(), Colors::Chocolate);

}

void PlayerBase::Render(DirectX::Model* model)
{
	model;

	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180));
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y - 1.5f, m_data.pos.z);

	m_baseModel_Towor->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(),
		false, [&]()
		{
			ModelShader::GetInstance().ModelDrawShader(
				SimpleMath::Color(Colors::Gray),
				SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), pSL.GetTowerBaseTexture());
			
			pSD.GetContext()->PSSetShaderResources(1, 1, pSL.GetTowerBaseTexture().GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, pSL.GetTowerBaseNormalMap().GetAddressOf());

			//　====================[　深度ステンシルステートの設定　]
			pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthDefault(), 0);

		});

	m_baseModel_Pillar->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(),
		false, [&]()
		{

			ModelShader::GetInstance().ModelDrawShader(
				SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f),
				SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SpriteLoder::GetInstance().GetTowerPillarTexture());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetTowerPillarTexture().GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetTowerPillarTexture().GetAddressOf());

			//　====================[　深度ステンシルステートの設定　]
			pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthDefault(), 0);

		});

}

void PlayerBase::WriteDepth(DirectX::Model* model)
{
	model;
	ShareData& pSD = ShareData::GetInstance();

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180));
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y - 1.5f, m_data.pos.z);

	m_baseModel_Towor->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(),
		false, [&]()
		{
			ModelShader::GetInstance().ShadowModelDraw(false);

		});

	m_baseModel_Pillar->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(),
		false, [&]()
		{

			ModelShader::GetInstance().ShadowModelDraw(false);

		});

}

void PlayerBase::Finalize()
{

	m_baseModel.reset();
	m_testBox.reset();

}

void PlayerBase::RenderBillboardUI()
{
	ShareData& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	SimpleMath::Vector2 billboardUIPosition = SimpleMath::Vector2(GetPos().x, GetPos().y);

	//　====================[　マシンのHPゲージ(外枠)　]
	SpriteLoder::TextureData texData = SpriteLoder::GetInstance().GetBaseGage();
	RECT rect = { 0,0,texData.width,texData.height };
	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		Colors::White,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
			static_cast<float>(texData.height / 2)),
		HPGAUGE_RAGE);

	// 描画位置の設定に使用
	float gaugeWidthHalf = static_cast<float>(texData.width / 2);

	//　====================[　拠点のHPゲージ(徐々に減少)　]
	texData = SpriteLoder::GetInstance().GetMainGage();
	rect = { 0,0,static_cast<LONG>(texData.width * m_difRedioHp),texData.height };

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition,
		&rect,
		Colors::HotPink,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
			static_cast<float>(texData.height / 2)),
		HPGAUGE_RAGE);

	//　====================[　拠点の魔法陣描画　]
	texData = SpriteLoder::GetInstance().GetMachineMagicCircleTexture(0);
	rect = { 0,0, texData.width,texData.height };

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		Colors::Aqua,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
			static_cast<float>(texData.height / 2)),
		MAGICCIRCLE_RAGE);

	//　====================[　拠点の魔法陣描画(経験値量描画)　]
	float ratioExp = 1 - (static_cast<float>(GetExp()) / static_cast<float>(GetNextLvEXP()));
	rect = { 0,0, texData.width, static_cast<LONG>(texData.height * ratioExp) };

	pSB->Draw(texData.tex.Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		Colors::White,
		0.0f,
		SimpleMath::Vector2(static_cast<float>(texData.width / 2),
			static_cast<float>(texData.height / 2)),
		MAGICCIRCLE_RAGE);


	//　====================[　拠点のLv描画　]
	rect = SpriteCutter((int)SPRITE_RAGE, (int)SPRITE_RAGE, m_baseLv, 0);
	pSB->Draw(SpriteLoder::GetInstance().GetNumberTexture().Get(),
		billboardUIPosition + SimpleMath::Vector3(-gaugeWidthHalf * HPGAUGE_RAGE, 0.0f, 0.0f),
		&rect,
		Colors::Black,
		0.0f,
		SimpleMath::Vector2(SPRITE_RAGE / 2, SPRITE_RAGE / 2),
		MACHINELV_RAGE);
}

int PlayerBase::GetNextLvEXP()
{
	auto gameParam = ShareJsonData::GetInstance().GetGameParameter();

	return gameParam.needExp + (gameParam.needExp_Growthrate * m_baseLv);
}

void PlayerBase::Damage(float damage)
{
	// 無敵時間が規定に達していればダメージを受ける
	if (m_invincibleTime >= 1.0f)
	{

		m_invincibleTime = 0.0f;
		DataManager::GetInstance()->SetNowBaseHP(DataManager::GetInstance()->GetNowBaseHP() - (int)damage);
	
	}
}
