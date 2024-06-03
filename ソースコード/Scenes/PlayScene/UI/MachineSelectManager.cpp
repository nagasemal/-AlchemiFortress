#include "pch.h"
#include "MachineSelectManager.h"
#include "Scenes/DataManager.h"

#include "Scenes/Commons/UIKeyControl.h"
#include "Scenes/PlayScene/UI/Number.h"

#include "Scenes/PlayScene/UI/DisplayMagicCircle.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/GameData/JsonLoder.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/SpriteCutter.h"

#include "NecromaLib/GameData/Particle_2D.h"

//　====================[　マシンUIのサイズ　]
#define MACHINE_UI_SIZE 0.23f

MachineSelectManager::MachineSelectManager() :
	m_selectMachineType(MACHINE_TYPE::NONE),
	m_selectBoxAll(false),
	m_manufacturingFlag(false),
	m_selectNoneFlag(false),
	m_forwardOrderFlag(false),
	m_rotateTime(),
	m_selectNumber(1),
	m_cursorMachineNumber(),
	m_prevSelectNumber(),
	m_uiTransparentTime(),
	m_alchemiActiveFlag(),
	m_automaticFlag(true)
{
}

MachineSelectManager::~MachineSelectManager()
{
}

void MachineSelectManager::TextuerLoader()
{
}

void MachineSelectManager::Initialize()
{
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	//　====================[　マシンUIの情報を取得　]
	UI_Data uiData = pSJD.GetUIData("AlchemiMachineIcon");

	SimpleMath::Vector2 spacePos = SimpleMath::Vector2(uiData.option["SPACE"], 0.0f);

	// Noneを省くために1スタート
	for (int i = 0; i < MACHINE_TYPE::NUM; i++)
	{

		m_machineSelect[i] = std::make_unique<MachineSelect>();

		m_machineSelect[i]->SetMachineType((MACHINE_TYPE)i);
		
		m_machineSelect[i]->SetPosition(uiData.pos - (spacePos * (MACHINE_TYPE::NUM / 2.0f)) + (spacePos * i));

		m_machineSelect[i]->Initialize();

		m_machineSelect[i]->GetMachineBox()->SetSavePos(m_machineSelect[i]->GetPos());

		m_machineSelect[i]->GetMachineBox()->SetKey(uiData.key);

	}

	//　====================[　メンバ変数のリセット　]
	m_selectBoxAll		= false;
	m_manufacturingFlag = false;

	m_alchemiActiveFlag = true;

	//　====================[　錬金ボタン選択時浮き出る魔法陣の取得　]
	m_displayMagicCircle = std::make_unique<DisplayMagicCircle>();

	//　====================[　マウスが周辺に位置するかを取得　]
	m_collider			= std::make_unique<SelectionBox>(uiData.pos,SimpleMath::Vector2(uiData.option["COLLIDER"], uiData.option["COLLIDER"]));
	m_collider			->Initialize();
	m_collider			->SetRect(RECT{ 0,0,1,1});
	m_collider			->SetWorldInfluenceFlag(false);

	//　====================[　設置モードUIの情報を取得　]
	uiData = pSJD.GetUIData("AlchemiMode");
	m_modeChangeButton = std::make_unique<SelectionBox>(uiData.pos, uiData.rage);
	m_modeChangeButton->Initialize();
	m_modeChangeButton->SetLayer((int)uiData.option["LAYER"]);
	m_modeChangeButton->SetKey(uiData.key);

	//　====================[　錬金ボタン内の所持数描画を取得　]
	uiData = pSJD.GetUIData("AlchemiNum");
	m_machineNumRender	= std::make_unique<Number>(uiData.pos, uiData.rage);

	//　====================[　錬金ボタン内の必要魔力量描画を取得　]
	uiData = pSJD.GetUIData("AlchemiMP");
	m_mpNumRender		= std::make_unique<Number>(uiData.pos, uiData.rage);

	//　====================[　錬金ボタン内の必要クリスタル描画を取得　]
	uiData = pSJD.GetUIData("AlchemiCrystal");
	m_crystalRender		= std::make_unique<Number>(uiData.pos, uiData.rage);

	//　====================[　パーティクルの生成　]
	m_alchemiCircle = std::make_unique<Particle_2D>();
	m_particle		= std::make_unique<Particle_2D>();

	//　====================[　チュートリアルの有無の変化　]
	if (pSJD.GetStageData().tutorial[0].type != 0)
	{
		// 手動設置モードに変更
		m_automaticFlag = true;

		// 一度全てアクティブフラグを切るようにする
		for (int i = 0; i < MACHINE_TYPE::NUM; i++)
		{
			m_machineSelect[i]->SetTutorialLockUI(false);
		}

	}

}

void MachineSelectManager::Update(FieldObjectManager* fieldObjectManager)
{

	auto datas = DataManager::GetInstance();
	auto pSJD = &ShareJsonData::GetInstance();
	auto pINP = &InputSupport::GetInstance();

	float deltaTime = DeltaTime::GetInstance().GetNomalDeltaTime();

	// マシンUIが選択されているならば対象のIDを入力
	// 選択されていない状態ならば規定位置に存在するマシンUIのIDを入力
	MACHINE_TYPE machineType = (MACHINE_TYPE)m_selectNumber;

	TransparentUI(1.0f);

	//　====================[　メンバ変数のリセット　]
	m_selectBoxAll		= false;
	m_manufacturingFlag = false;

	//　====================[　パーティクルの更新　]
	m_alchemiCircle->UpdateParticle();
	m_particle		->UpdateParticle();

	//　====================[　各マシンUIの更新　]
	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Update();

		if (m_machineSelect[i]->GetMachineBox()->HitMouse())
		{
			m_selectNumber = (int)m_machineSelect[i]->GetMachineType();
		}

	}

	//　====================[　マシンUIに触れた際の処理　]
	//　　|=>　触れていたらUI出現用の時間変数に時間加算 触れてなければ減算
	if (m_machineSelect[m_selectNumber]->GetHitMouseFlag()) 	m_textAnim += deltaTime;
	else 	m_textAnim -= deltaTime;

	//　====================[　フィールドのクリスタル選択時　]
	//　　|=>　エクスカベーターUIの色を変える
	m_machineSelect[MACHINE_TYPE::MINING]->SetChangeColorFlag(fieldObjectManager->GetCrystalToMouse());
	//　　|=>　更新
	m_displayMagicCircle->Update();
	m_displayMagicCircle->TransparentUpdate(m_selectBoxAll);

	RotationMachineList();

	// 錬金ボタンが押された
	if (m_machineSelect[machineType]->GetMachineBox()->ClickMouse())
	{

		// 錬金された瞬間を取得
		m_manufacturingFlag = true;

		// 錬金された瞬間のマシンタイプを取得する
		m_selectMachineType = m_machineSelect[machineType]->GetMachineType();

		// マシンタイプを取得しリソース群を減らす
		ReduceResource(machineType);

		m_alchemiCircle	->OnShot("AlchemiCircle", m_machineSelect[machineType]->GetMachineBox()->GetPos(),true);
		m_particle		->OnShot("AlchemiParticle", m_machineSelect[machineType]->GetMachineBox()->GetPos(), true);

	}

	// モード変更
	m_modeChangeButton->HitMouse();
	if (m_modeChangeButton->ClickMouse())
	{
		m_automaticFlag = !m_automaticFlag;
	}

}

void MachineSelectManager::Render()
{
}

void MachineSelectManager::RenderUI(int machineNum[])
{

	SpriteLoder& pSL	= SpriteLoder::GetInstance();
	auto pSB			= ShareData::GetInstance().GetSpriteBatch();
	ShareData& pSD		= ShareData::GetInstance();
	auto pINP			= &InputSupport::GetInstance();
	ShareJsonData& pSJD = ShareJsonData::GetInstance();
	UI_Data uiData		= UI_Data();

	//　====================[　画像切り取り変数の定義　]
	RECT rect = RECT();

	//　====================[　マシンアイコンUIの描画　]
	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->DisplayObject();
	}

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	//　====================[　マウス周辺に描画するマシンアイコン　]
	//　　|=>　現在マウスがUIに触れていない
	if (!pINP->GetHitUI())
	{
		//　　|=>　色を設定
		SimpleMath::Color name_color = SimpleMath::Color(Colors::White);

		//　　|=>　所持数が0ならば色を変更する
		if (machineNum[m_selectNumber] <= 0) name_color = Colors::Red;

		//　====================[　マシンUIを描画　]
		SpriteLoder::TextureData texData = pSL.GetMachineIconTexture();
		rect = SpriteCutter(texData.width / (MACHINE_TYPE::NUM - 1), texData.height, m_selectNumber - 1, 0);

		pSB->Draw(texData.tex.Get(), pINP->GetMousePosScreen(),
			&rect, name_color, 0.0f,
			DirectX::XMFLOAT2(static_cast<float>((texData.width / (MACHINE_TYPE::NUM - 1)) / 2),
				static_cast<float>(texData.height / 2)), MACHINE_UI_SIZE);

	}

	//　====================[　マシンの一行説明文]
	SpriteLoder::TextureData texData = pSL.GetMachineSimpleText();
	rect = SpriteCutter(texData.width / (MACHINE_TYPE::NUM - 1), texData.height, m_selectNumber - 1, 0);

	pSB->Draw(texData.tex.Get(), pSJD.GetUIData("AlchemiText").pos,
		&rect, Colors::White, 0.0f,
		DirectX::XMFLOAT2(static_cast<float>((texData.width / (MACHINE_TYPE::NUM - 1)) / 2),
			static_cast<float>(texData.height / 2)), pSJD.GetUIData("AlchemiText").rage);

	pSB->End();

	//　====================[　所持数の描画(錬金ボタン内部)　]
	if (!m_automaticFlag)
	{
		m_machineNumRender->SetNumber(machineNum[m_selectNumber]);
		m_machineNumRender->SetPosition(m_machineSelect[m_selectNumber]->GetPos());
		m_machineNumRender->Render();
	}

	//　====================[　設置モード変更ボタン描画　]
	SpriteLoder::TextureData alchemiArrowTexture = pSL.GetInstallationMode();
	m_modeChangeButton->DrawUI(alchemiArrowTexture.tex.Get(), 0.0f, alchemiArrowTexture.width / 2, alchemiArrowTexture.height, (int)!m_automaticFlag, 0);
	m_modeChangeButton->SetRect(RECT{ 0,0, alchemiArrowTexture.width / 2, alchemiArrowTexture.height });

	//　====================[　パーティクルの描画　]
	SpriteLoder::TextureData particleTex = pSL.GetMachineMagicCircleTexture(m_selectNumber);
	m_alchemiCircle->Render(particleTex.tex, RECT{ 0,0, particleTex.width, particleTex.height});

	particleTex = pSL.GetNormalParticle();
	m_particle		->Render(particleTex.tex, RECT{ 0,0, particleTex.width, particleTex.height });

}

// 呼び出し元で要素分回している
void MachineSelectManager::ModelRender(int index)
{
	m_machineSelect[index]->DisplayObject();
}

void MachineSelectManager::MagicCircleRender()
{
	m_displayMagicCircle->SpritebatchRender();
}

void MachineSelectManager::Finalize()
{

	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->Finalize();
	}

}

void MachineSelectManager::ReduceResource(MACHINE_TYPE type)
{
	auto datas = DataManager::GetInstance();
	auto pSJD = &ShareJsonData::GetInstance();

	datas->SetNowMP(datas->GetNowMP() - pSJD->GetMachineData(type).alchemi_mp);

	datas->SetNowCrystal(datas->GetNowCrystal() - pSJD->GetMachineData(type).alchemi_crystal);

}

void MachineSelectManager::RotationMachineList()
{
	// 規定値ならば処理を行わない
	if (m_rotateTime >= 1.0f)
	{
		m_rotateTime = 1.0f;
		return;
	}

	// 回転を行う
	m_rotateTime += DeltaTime::GetInstance().GetNomalDeltaTime() * 6.0f;
}

void MachineSelectManager::LRButtonLock(bool flag)
{
}

void MachineSelectManager::AlchemiButtonLock(bool flag)
{
	m_alchemiActiveFlag = flag;
}

void MachineSelectManager::TransparentUI(float transparentVal)
{
	auto datas = DataManager::GetInstance();
	auto pSJD = &ShareJsonData::GetInstance();

	// マシンUIが選択されているならば対象のIDを入力
	// 選択されていない状態ならば規定位置に存在するマシンUIのIDを入力
	MACHINE_TYPE machineType = (MACHINE_TYPE)m_selectNumber;

	//　====================[　所持数が0以下である場合、数字を赤くする　] 
	//　　|=>　マシン
	if (m_machineNumRender->GetNumber() <= 0)
	{
		m_machineNumRender->SetColor(SimpleMath::Color(Colors::Red));
	}
	else
	{
		m_machineNumRender->SetColor(SimpleMath::Color(Colors::Black));
	}
	//　　|=>　魔力
	if (pSJD->GetMachineData(machineType).alchemi_mp > datas->GetNowMP())
	{
		m_mpNumRender->SetColor(SimpleMath::Color(Colors::Red));
	}
	else
	{
		m_mpNumRender->SetColor(SimpleMath::Color(Colors::Black));
	}
	//　　|=>　結晶
	if (pSJD->GetMachineData(machineType).alchemi_crystal > datas->GetNowCrystal())
	{
		m_crystalRender->SetColor(SimpleMath::Color(Colors::Red));
	}
	else
	{
		m_crystalRender->SetColor(SimpleMath::Color(Colors::Black));
	}

	//　====================[　透明化を行う　]
	m_machineNumRender	->SetColor(SimpleMath::Color(m_machineNumRender	->GetColorRGB(), transparentVal));
	m_mpNumRender		->SetColor(SimpleMath::Color(m_mpNumRender		->GetColorRGB(), transparentVal));
	m_crystalRender		->SetColor(SimpleMath::Color(m_crystalRender	->GetColorRGB(), transparentVal));

	// Noneを省くために1スタート
	for (int i = 1; i < MACHINE_TYPE::NUM; i++)
	{
		m_machineSelect[i]->GetMachineBox()->SetColor(SimpleMath::Color(m_machineSelect[i]->GetMachineBox()->GetColorRGB(), transparentVal));
	}
}
