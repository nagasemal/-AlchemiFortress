//--------------------------------------------------------------------------------------
// File: ShaderButton.h
//
//　シェーダーを適応したボタン描画に用いる
// 
// Date: 2024.1.11
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "NecromaLib/GameData/ShaderTemplate.h"

class ShaderButton : public ShaderTemplate
{
private:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		SimpleMath::Vector4	windowSize;
		SimpleMath::Vector4	color;
		SimpleMath::Vector4	diff;
	};

public:
	ShaderButton();
	~ShaderButton();

	void Update();
	void Render();

private:

};