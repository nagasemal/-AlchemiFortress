#include "pch.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "MachineSelect.h"
#include "NecromaLib/GameData/ScreenToWorld.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/Singleton/ShareJsonData.h"

//　====================[　ボタンUIの大きさ　]
#define SELECTBOX_RAGE		{ 1.3f,1.3f }

//　====================[　画像の大きさ　]
#define IMAGE_WIGHT			64
#define IMAGE_HEIGHT		64

#define ALPHA_VAL 0.7f
#define SCALE_VAL 0.35f

//　====================[　魔法陣の回転速度　]
#define MAGICCIRCLE_SPEED 0.5f

//　====================[　魔法陣の透明度　]
#define MAGICCIRCLE_ALPHA 0.25f

MachineSelect::MachineSelect():
	m_hitMouseFlag(),
	m_onMouseFlag(),
	m_manufacturingFlag(),
	m_selectMachineType(MACHINE_TYPE::NONE),
	m_changeColorFlag(),
	m_colorChangeTime(),
	m_tutorialLockFlag(true)
{
}

MachineSelect::~MachineSelect()
{
}

void MachineSelect::Initialize()
{
	m_data.rage = SELECTBOX_RAGE;

	m_machineBox = std::make_unique<SelectionBox>(m_data.pos, m_data.rage);
	m_machineBox->Initialize();
	m_machineBox->SetColor((SimpleMath::Color)Colors::White);

	m_colorChangeTime = 0;

	//　====================[　錬金ボタンの情報を取得　]
	m_selectionAlchemi = std::make_unique<SelectionBox>(m_data.pos, m_data.rage * 0.5f);
	m_selectionAlchemi->Initialize();

}

void MachineSelect::Update()
{
	auto datas = DataManager::GetInstance();
	auto pSJD = &ShareJsonData::GetInstance();

	float deltaTime = DeltaTime::GetInstance().GetNomalDeltaTime();

	m_manufacturingFlag = false;

	m_machineBox->SetSavePos(m_data.pos);


	// 要素から製造ボタンが押された判定を受け取る リソースが足りない場合は弾く
	m_machineBox->SetActiveFlag(
		((datas->GetNowMP() - pSJD->GetMachineData(m_selectMachineType).alchemi_mp >= 0 &&
		  datas->GetNowCrystal() - pSJD->GetMachineData(m_selectMachineType).alchemi_crystal >= 0) && m_tutorialLockFlag));

	// リストの中から選ばれた
	m_machineBox->HitMouse(true);

	if (m_machineBox->ClickMouse())
	{
		m_onMouseFlag = !m_onMouseFlag;
	}

	//　====================[　錬金ボタンに触れた際の処理　]
	if (m_selectionAlchemi->HitMouse(true)) 					m_magicCircleAnim += deltaTime;
	else 	m_magicCircleAnim -= deltaTime;

}

void MachineSelect::Draw()
{
}

void MachineSelect::Finalize()
{
}

void MachineSelect::DisplayObject()
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	//　====================[　魔法陣を回転させる変数　]
	float easingValRotate = Easing::EaseOutQuint(0.0f, XMConvertToRadians(360.0f), m_magicCircleAnim);

	//　====================[　錬金ボタンの背面描画　]
	SpriteLoder::TextureData alchemiTexture_Back = pSL.GetAlchemiButtonBackTexture();
	m_selectionAlchemi->DrawUI(alchemiTexture_Back.tex.Get(), easingValRotate);


	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT srcRect = { 0, 0, IMAGE_WIGHT, IMAGE_HEIGHT };

	// ログの色
	SimpleMath::Color color = m_machineBox->GetColor();

	// 内部BOX (マシンUIを配置する箇所)
	SpriteLoder::TextureData texData = pSL.GetMachineIconTexture();

	srcRect = SpriteCutter(texData.width / (MACHINE_TYPE::NUM - 1), texData.height, m_selectMachineType - 1, 0);

	pSB->Draw(texData.tex.Get(), m_data.pos, &srcRect, color, 0.0f, 
		DirectX::XMFLOAT2(static_cast<float>((texData.width / (MACHINE_TYPE::NUM - 1)) / 2),
						  static_cast<float>(texData.height / 2)), SCALE_VAL);
	
	pSB->End();

	//　====================[　錬金ボタンの描画　]
	float colorElement = 0.5f + (float)m_machineBox->GetActiveFlag();
	color = SimpleMath::Color(colorElement, colorElement, colorElement, 1.0f);

	SpriteLoder::TextureData alchemiTexture = pSL.GetAlchemiButtonTexture();
	m_selectionAlchemi->SetPos(GetPos());
	m_selectionAlchemi->SetColor(color);
	m_selectionAlchemi->DrawUI(alchemiTexture.tex.Get(), 0.0f);
	m_selectionAlchemi->SetRect(RECT{ 0,0,alchemiTexture.width,alchemiTexture.height }); 

	//　====================[　錬金ボタン魔法陣の描画　]
	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());
	
	SpriteLoder::TextureData textureData = pSL.GetMachineMagicCircleTexture(m_selectMachineType);
	srcRect = { 0,0,textureData.width,textureData.height };
	pSB->Draw(textureData.tex.Get(), m_selectionAlchemi->GetPos(),
		&srcRect, SimpleMath::Color(1.0f, colorElement, colorElement, m_magicCircleAnim * MAGICCIRCLE_ALPHA), easingValRotate,
		SimpleMath::Vector2(textureData.width / 2.0f, textureData.height / 2.0f), m_magicCircleAnim * 0.1f);

	//　====================[　名前テクスチャの描画　]
	textureData = pSL.GetMachineNameTexture();
	int rectX = textureData.width / MACHINE_TYPE::NUM;
	srcRect = { rectX * m_selectMachineType,0,rectX * (m_selectMachineType + 1),textureData.height };
	pSB->Draw(textureData.tex.Get(), m_selectionAlchemi->GetPos(),
		&srcRect, SimpleMath::Color(1.0f, colorElement, colorElement,1.0f), 0.0f,
		SimpleMath::Vector2(rectX / 2.0f, textureData.height / 2.0f), m_magicCircleAnim);

	pSB->End();
}

void MachineSelect::LeftRender()
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT srcRect = { 0, 0, IMAGE_WIGHT, IMAGE_HEIGHT };

	// ログの色
	SimpleMath::Color color = m_machineBox->GetColor();

	color.A(ALPHA_VAL);

	// 内部BOX (マシンUIを配置する箇所)
	SpriteLoder::TextureData texData = pSL.GetMachineIconTexture();

	srcRect = SpriteCutter(texData.width / (MACHINE_TYPE::NUM - 1), texData.height, m_selectMachineType - 1, 0);

	pSB->Draw(texData.tex.Get(), m_data.pos + SimpleMath::Vector2(160.0f,0.0f), &srcRect, color, 0.0f,
		DirectX::XMFLOAT2(static_cast<float>((texData.width / (MACHINE_TYPE::NUM - 1)) / 2),
			static_cast<float>(texData.height / 2)), SCALE_VAL);

	pSB->End();
}

void MachineSelect::RightRender()
{
	ShareData& pSD = ShareData::GetInstance();
	SpriteLoder& pSL = SpriteLoder::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT srcRect = { 0, 0, IMAGE_WIGHT, IMAGE_HEIGHT };

	// ログの色
	SimpleMath::Color color = m_machineBox->GetColor();

	color.A(ALPHA_VAL);

	// 内部BOX (マシンUIを配置する箇所)
	SpriteLoder::TextureData texData = pSL.GetMachineIconTexture();

	srcRect = SpriteCutter(texData.width / (MACHINE_TYPE::NUM - 1), texData.height, m_selectMachineType - 1, 0);

	pSB->Draw(texData.tex.Get(), m_data.pos - SimpleMath::Vector2(160.0f, 0.0f), &srcRect, color, 0.0f,
		DirectX::XMFLOAT2(static_cast<float>((texData.width / (MACHINE_TYPE::NUM - 1)) / 2),
			static_cast<float>(texData.height / 2)), SCALE_VAL);

	pSB->End();
}

bool MachineSelect::SelectUIFlag()
{
	return m_onMouseFlag;
}

