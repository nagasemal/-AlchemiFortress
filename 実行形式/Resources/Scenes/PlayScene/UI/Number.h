//--------------------------------------------------------------------------------------
// File		: Number.h
//
// スプライト画像による数字の描画に使用
// 
// Use		: MachineSelect AlchemicalMachineObject
//			  AM_Attacker AM_Upeer
// Date		: 2023.7.17
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "NecromaLib/GameData/GameObject2D.h"

class Number
{
public:
	Number();
	Number(SimpleMath::Vector2 position,SimpleMath::Vector2 rage);
	~Number();

	// 表示する数字の設定
	void SetNumber(int num);
	void SetPosition(SimpleMath::Vector2 position) { m_position = position;	m_position.x += 32 * m_rage.x; }
	void SetRage(SimpleMath::Vector2 rage) { m_rage = rage; }

	void SetColor(SimpleMath::Color color) { m_color = color; }

	// 数字画像描画
	void Render();

	// スプライトバッチのbegin関数を呼ばない
	void Render_NoSpriteBatch();

	/// <summary>
	/// ○/○等の表現が可能
	/// </summary>
	/// <param name="first">分子</param>
	/// <param name="next">分母</param>
	/// <param name="symbol">0 = / , 1 = :</param>
	void Render_SelectScene(int first,int next,int symbol = 0);

	// 入れられた数字を取得する
	int GetNumber() { return m_num; }

	SimpleMath::Vector2 GetPosition() { return m_position; }
	SimpleMath::Vector2 GetRage() { return m_rage; }

	// 色情報を返す
	SimpleMath::Color GetColor() { return m_color; }
	// 色情報(RGB)を返す
	SimpleMath::Vector3 GetColorRGB() { return SimpleMath::Vector3(m_color.R(), m_color.G(), m_color.B()); }

private:

	// 数字を描画する
	void Number_Render(int num,SimpleMath::Vector2 pos);

	SimpleMath::Vector2 m_position;
	SimpleMath::Vector2 m_rage;

	SimpleMath::Color m_color;

	int m_num;

	int m_prevNum;

	float m_animationPosTime;

	bool m_animationFlag;

};