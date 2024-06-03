//--------------------------------------------------------------------------------------
// File		: SelectionBox.h
//
// オブジェクト選択時等に現れる選択可能UI
// 基本機能 触れる　クリック　ホールド　ボタン式　離れた瞬間　 の取得
// 
// Use		: MachineSelect AlchemicalMachineObject
//			  AM_Attacker AM_Upeer
// Date		: 2023.7.12
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "Scenes/Commons/SelectionUI.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

class SelectionBox : public SelectionUI
{
public:
	SelectionBox(SimpleMath::Vector2 pos,SimpleMath::Vector2 rage);
	~SelectionBox();

	void Update()				override;
	// 描画
	void Draw()					override;

	void DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture = nullptr,
		RECT rect = { 0,0,64,64 },
		SimpleMath::Color pulsTextureCol = { 1.0f,1.0f,1.0f,1.0f }, float pulsRage = 0.8f,int pulsWidth = 64, int pulsHeight = 64);

	// 画像の拡大無しバージョン
	void DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,float rotate);

	// 中身がUIアイコンである場合に使用
	void DrawUI(int UInumber);

	void DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture = nullptr,
		RECT rect = { 0,0,64,64 },float pulsRage = 0.8f,
		SimpleMath::Color pulsTextureCol = {1.0f,1.0f,1.0f,1.0f},
		SimpleMath::Color nomalColor	 = {1.0f,1.0f,1.0f,1.0f},
		SimpleMath::Color onColor		 = {0.9f,0.9f,0.9f,1.0f},
		SimpleMath::Color pressdColor	 = {0.7f,0.7f,0.7f,1.0f});

	void DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
		RECT rect							= { 0,0,64,64 },
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pulsTexture = nullptr,
		RECT pulsTexRect					= { 0,0,64,64 },
		SimpleMath::Color pulsTextureCol	= { 1.0f,1.0f,1.0f,1.0f },
		SimpleMath::Color nomalColor		= { 1.0f,1.0f,1.0f,1.0f },
		SimpleMath::Color onColor			= { 0.9f,0.9f,0.9f,1.0f },
		SimpleMath::Color pressdColor		= { 0.7f,0.7f,0.7f,1.0f });

	void DrawUI(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,float rotate, int width, int height, int beside, int vertical);

private:

	float m_popUITextTimer;

	std::unique_ptr<DrawKey> m_drawKey;

};
