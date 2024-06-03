#pragma once
#include "NecromaLib/GameData/ShaderTemplate.h"
#include "NecromaLib/GameData/Animation.h"

class Veil :public ShaderTemplate
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		SimpleMath::Vector4	windowSize;
		SimpleMath::Vector4	color;
		SimpleMath::Vector4	diff;
	};

public:
	Veil(int ruleType);
	~Veil();

	void Update();
	void Render();

	bool GetAnimationFin() { return m_animData[1].MaxCheck(); }

private:

	SimpleMath::Vector4 m_animationTime;

	// アニメーション用の時間遷移を行うクラス
	AnimationData m_animData[2];

	int m_rule;

};