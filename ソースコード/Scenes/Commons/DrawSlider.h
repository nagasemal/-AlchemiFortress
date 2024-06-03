//--------------------------------------------------------------------------------------
// File: DrawUserBar.h
//
// ユーザー側が操作できるバーの処理、描画
// 
// Date: 2023.9.15
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/UserInterfase.h"

class SelectionBox;
class DrawSlider
{
public:
	DrawSlider(SimpleMath::Vector2 position, SimpleMath::Vector2 scale);
	~DrawSlider();

	void Initialize();
	void Update();
	void Render(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr,RECT rect ={0,0,64,64});
	void Render(int uiIcon);
	void Finalize();

	const float GetValue() const { return m_value;}

private:

	void CreateBar(SimpleMath::Vector2 position, SimpleMath::Vector2 scale);

	// ゲージ前景
	std::unique_ptr<UserInterface> m_gauge_Flont;
	// ゲージ背景
	std::unique_ptr<UserInterface> m_gauge_Back;
	// ツマミ
	std::unique_ptr<SelectionBox> m_button;

	float m_value;

	// 画像の両最端位置
	SimpleMath::Vector2 m_maxPos, m_minPos;

};