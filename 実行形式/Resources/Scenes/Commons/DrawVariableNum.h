//--------------------------------------------------------------------------------------
// File: DrawVariableNum.h
//
//　プレイヤーが可変可能な数字の描画
// 
// Date: 2023.10.20
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class SelectionBox;
class DrawArrow;
class Number;
class DrawVariableNum
{
public:
	DrawVariableNum(SimpleMath::Vector2 position, SimpleMath::Vector2 rage);
	~DrawVariableNum();

	void Update(int minVal = 0,int maxVal = 99);
	void Render();

	int GetNumber();
	void SetNumber(int number);

	SelectionBox* GetSelectionBox() { return m_box.get(); }

private:

	std::unique_ptr<SelectionBox>	m_box;
	std::unique_ptr<DrawArrow>	m_leftArrow, m_rightArrow;
	std::unique_ptr<Number>		m_number;

};