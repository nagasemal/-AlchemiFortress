#include "pch.h"
#include "SpriteLoder.h"
#include "ShareData.h"
#include <WICTextureLoader.h>

#include "NecromaLib/GameData/CommonStruct.h"

SpriteLoder* SpriteLoder::instance = nullptr;

void SpriteLoder::Loading()
{

	// 選択ボックスのテクスチャ
	LoadingPngFile(L"Resources/Textures/log.png", m_selectBoxTexture);

	// 錬金用選択ボタンの取得
	LoadingPngFileData(L"Resources/Textures/AlchemiButton.png",m_alchemiSelectTexture);

	// 錬金用選択ボタンの背面の取得
	LoadingPngFileData(L"Resources/Textures/AlchemiButtonBack.png", m_alchemiBackTexture);

	// 錬金用矢印ボタンの取得
	LoadingPngFileData(L"Resources/Textures/AlchemiButtonArrow.png", m_alchemiArrowTexture);

	LoadingPngFileData(L"Resources/Textures/BottomLabel.png", m_bottomLabel);

	LoadingPngFile(L"Resources/Textures/SelectMachineUICursor.png", m_machineUICursorTexture);

	LoadingPngFile(L"Resources/Textures/EnemyIcon.png", m_enemyStateTexture);

	// マウステクスチャの取得
	LoadingPngFile(L"Resources/Textures/Mouse.png", m_mouseTexture);
	
	LoadingPngFile(L"Resources/Textures/GameStart.png", m_gameStartTexture);

	//　====================[　マシンのノーマルマップ取得　]
	LoadingPngFile(L"Resources/Textures/NormalMap.png", m_machineNormalMap[0]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Fire_NormalMap.png", m_machineNormalMap[1]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Ice_NormalMap.png", m_machineNormalMap[2]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Wind_NormalMap.png", m_machineNormalMap[3]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Earth_NormalMap.png", m_machineNormalMap[4]);

	//　====================[　マシンのテクスチャ取得　]
	LoadingPngFile(L"Resources/Textures/NormalMap.png", m_machineTexture[0]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Fire_Texture.png", m_machineTexture[1]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Ice_Texture.png", m_machineTexture[2]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Wind_Texture.png", m_machineTexture[3]);
	LoadingPngFile(L"Resources/Textures/ModelTexture/Earth_Texture.png", m_machineTexture[4]);

	//　====================[　フィールドのノーマルマップ取得　]
	LoadingPngFile(L"Resources/Textures/ModelTexture/StornTile_Nomal.png", m_fieldNormalMap);
	//　====================[　フィールドのテクスチャ取得　]
	LoadingPngFile(L"Resources/Textures/ModelTexture/StornTile.png", m_fieldTexture);

	//　====================[　塔[ベース]のノーマルマップ取得　]
	LoadingPngFile(L"Resources/Textures/ModelTexture/Diriseki_Nomal.png", m_towerNormalMap);
	//　====================[　塔[ベース]のテクスチャ取得　]
	LoadingPngFile(L"Resources/Textures/ModelTexture/Diriseki.jpg", m_towerTexture);

	//　====================[　塔[柱]のノーマルマップ取得　]
	LoadingPngFile(L"Resources/Textures/ModelTexture/Pillar_Nomal.png", m_towerPillarNormalMap);
	//　====================[　塔[柱]のテクスチャ取得　]
	LoadingPngFile(L"Resources/Textures/ModelTexture/Pillar.jpg", m_towerPillarTexture);



	LoadingPngFile(L"Resources/Textures/Effect/Aura.png", m_auraBaseTexture);
	LoadingPngFile(L"Resources/Textures/Effect/Flare.png", m_auraMaskTexture);

	// テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/elementTex.png",
		nullptr,
		m_elementTextures.ReleaseAndGetAddressOf()
	);

	// エネミーの名前のテクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/EnemyNames.png",
		nullptr,
		m_enemyNameTextures.ReleaseAndGetAddressOf()
	);

	// 製造の文字テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Seizou.png",
		nullptr,
		m_ManufacturingTexture.ReleaseAndGetAddressOf()
	);

	// 文字テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/wo.png",
		nullptr,
		m_letterTexture.ReleaseAndGetAddressOf()
	);

	// 文字テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MissionText.png",
		nullptr,
		m_missionTexture.ReleaseAndGetAddressOf()
	);

	// リザルトにて描画する文字テクスチャ
	LoadingPngFile(L"Resources/Textures/Result_Text.png", m_resultTextTexture);

	LoadingPngFile(L"Resources/Textures/SuccessFailureText.png", m_stageClearTextTexture);

	// ラベルテクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/MissionLabel.png",
		nullptr,
		m_missionLabelTexture.ReleaseAndGetAddressOf()
	);
	
	// 数字テクスチャ
	LoadingPngFile(L"Resources/Textures/number_1.png",m_NumberTexture);

	// 記号テクスチャ
	LoadingPngFile(L"Resources/Textures/symbol.png", m_SymbolTexture);

	// タイトルロゴテクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/TitleLogo.png",
		nullptr,
		m_titleLogo.ReleaseAndGetAddressOf()
	);

	LoadingPngFile(L"Resources/Textures/Explanation_Text.png", m_explanationTextTexture);

	// UIアイコンテクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/UIIcons.png",
		nullptr,
		m_uiIcons.ReleaseAndGetAddressOf()
	);

	// UIアイコンテクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Arrow.png",
		nullptr,
		m_uiArrow.ReleaseAndGetAddressOf()
	);

	// ルール画像テクスチャ
	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		L"Resources/Textures/Rule.png",
		nullptr,
		m_rule.ReleaseAndGetAddressOf()
	);

	LoadingPngFile(L"Resources/Textures/Rule/Right_Transition.png", m_transition[0]);
	LoadingPngFile(L"Resources/Textures/Rule/UP_Transition.png", m_transition[1]);
	LoadingPngFile(L"Resources/Textures/Rule/109.png", m_transition[2]);
	LoadingPngFile(L"Resources/Textures/Rule/156.png", m_transition[3]);

	LoadingPngFile(L"Resources/Textures/TitleText.png", m_pleaseMouseButtonText);

	LoadingPngFile(L"Resources/Textures/SkySphereTexture.png", m_skySphereTexture);

	LoadingPngFile(L"Resources/Textures/RampTexture.png", m_rampTexture);


	//　====================[　マシンの魔法陣　]
	{
		LoadingPngFileData(L"Resources/Textures/MagicCircle/MajicCircle.png", m_magicCircleTexture[MACHINE_TYPE::NONE]);

		for (int i = 1; i < MACHINE_TYPE::NUM; i++)
		{
			// モデル名前配列に応じたファイルパスを読み込む
			std::wostringstream oss;
			oss << L"Resources/Textures/MagicCircle/" << ChangeData::ChangeMachineString((MACHINE_TYPE)i).c_str() << ".png";

			LoadingPngFileData(oss.str().c_str(), m_magicCircleTexture[i]);
		}

		LoadingPngFileData(L"Resources/Textures/MagicCircle/EnemySpawn.png", m_magicCircleTexture[6]);
	}

	//　====================[　マシンUIのテキスト　]
	{

		LoadingPngFileData(L"Resources/Textures/UIText/AttackerText.png", m_machineUIText[MACHINE_TYPE::NONE]);

		for (int i = 1; i < MACHINE_TYPE::NUM; i++)
		{
			// モデル名前配列に応じたファイルパスを読み込む
			std::wostringstream oss;
			oss << L"Resources/Textures/UIText/" << ChangeData::ChangeMachineString((MACHINE_TYPE)i).c_str() << "Text.png";

			LoadingPngFileData(oss.str().c_str(), m_machineUIText[i]);
		}

	}

	//　====================[　アルファベットのテクスチャ　]
	LoadingPngFileData(L"Resources/Textures/UIText/Alphabet.png", m_alphabet);

	//　====================[　UIテクスチャ　]
	LoadingPngFileData(L"Resources/Textures/UIText/UIText.png", m_uiText);

	//　====================[　ゲージ　]
	//　　|=>　背景
	LoadingPngFileData(L"Resources/Textures/Gauge_base.png", m_baseGageTex);
	//　　|=>　中身
	LoadingPngFileData(L"Resources/Textures/Gauge_main.png", m_mainGageTex);

	//　====================[　マシンUIアイコン　]
	LoadingPngFileData(L"Resources/Textures/MachineIcons.png", m_machineIconTexture);

	//　====================[　マシン名前テクスチャ　]
	LoadingPngFileData(L"Resources/Textures/MachineNames.png", m_machineNameTextures);

	//　====================[　矢印アイコン　]
	LoadingPngFileData(L"Resources/Textures/InstallationMode.png", m_installationMode);

	//　====================[  マシンの一行説明　]
	LoadingPngFileData(L"Resources/Textures/MachineSimpleText.png", m_machineSimpleText);

	//　====================[　クリックポインター　]
	LoadingPngFileData(L"Resources/Textures/ClickPointer.png", m_clickMousePointer);

	//　====================[　パーティクル　]
	LoadingPngFileData(L"Resources/Textures/Particle.png", m_particle);

}

void SpriteLoder::Finalize()
{
	// 解放
	m_selectBoxTexture.Reset();
	m_elementTextures.Reset();
	m_ManufacturingTexture.Reset();
	m_NumberTexture.Reset();
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SpriteLoder::LoadingPngFile
(const wchar_t* filename, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& spriteResource)
{
		DirectX::CreateWICTextureFromFile(
			ShareData::GetInstance().GetDevice(),
			filename,
			nullptr,
			spriteResource.ReleaseAndGetAddressOf()
		);

	return spriteResource;
}

SpriteLoder::TextureData SpriteLoder::LoadingPngFileData(const wchar_t* filename, TextureData& textuerData)
{
	Microsoft::WRL::ComPtr<ID3D11Resource> res;

	DirectX::CreateWICTextureFromFile(
		ShareData::GetInstance().GetDevice(),
		filename,
		res.ReleaseAndGetAddressOf(),
		textuerData.tex.ReleaseAndGetAddressOf()
	);


	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(res.As(&tex));

	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	textuerData.width = desc.Width;
	textuerData.height = desc.Height;

	return textuerData;
}

SpriteLoder::SpriteLoder()
{

}

void SpriteLoder::Create()
{
	if (instance == nullptr)
	{
		instance = new SpriteLoder;
	}
}

void SpriteLoder::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}