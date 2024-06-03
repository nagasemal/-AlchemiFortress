#pragma once
#include "NecromaLib/GameData/ShaderTemplate.h"

class FieldObjectManager;
class Number;

class BaseLv : public ShaderTemplate
{
private:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		SimpleMath::Vector4	windowSize;
		SimpleMath::Vector4	base_color;
		SimpleMath::Vector4	second_color;
	};

public:
	BaseLv();
	~BaseLv();

	void Update(FieldObjectManager* pFieldObjectManager);
	void Render(float alphaVal);

private:

	// 拠点Lvの描画
	std::unique_ptr<Number> m_baseLvRender;

	// expの割合
	float m_expRedio;

};