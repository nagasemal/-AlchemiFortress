#pragma once
#include "NecromaLib/GameData/Animation.h"

class DisplayMagicCircle
{
public:
	DisplayMagicCircle();
	~DisplayMagicCircle();

	void Update();
	void Render();

	// 苦肉の策
	void SpritebatchRender();

	void SetWindowSize(const int& width, const int& height);

	void SetScale(SimpleMath::Vector2 scale);
	void SetPosition(SimpleMath::Vector2 position);

	void SetColor(SimpleMath::Color color)	{ m_color = color; }
	SimpleMath::Color GetColor()			{ return m_color; }

	void SetRotationMatrix(SimpleMath::Matrix matrix) { m_rotationMatrix = matrix;}

	// 魔法陣を出現させる
	void TransparentUpdate(bool spawnFlag);

private:

	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;

	SimpleMath::Vector2 m_scale;
	SimpleMath::Vector2 m_baseScale;
	SimpleMath::Vector2 m_position;
	SimpleMath::Color m_color;
	SimpleMath::Matrix	m_rotationMatrix;

	// 魔法陣を回す時間計測変数
	float m_rotateTime;

	// 魔法陣を透明→半透明に出現させる時間計測変数
	AnimationData m_transparentTime;
	float m_easing;
};