//--------------------------------------------------------------------------------------
// File: Explanation.h
//
//　操作ガイドの描画
// 
// Date: 2023.10.25
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class AlchemicalMachineManager;

class Explanation
{
private:

	enum EXPLANATION_TYPE :int
	{
		MACHINE_SELECT		 = 0, // マシン　選択
		MACHINE_INSTALLATION = 1, // マシン　設置

		ZOOM				 = 2, // カメラズーム
		MACHINE_LINE		 = 3, // マシン設置のライン決め

		ROTATE_STOP			 = 4, // 回転停止
		ROTATE_START		 = 5  // 回転再開

	};

public:
	Explanation();
	~Explanation();

	void Update();

	void Render(bool selectNowFlag,bool rotateStopFlag);

	SimpleMath::Vector2 RightExplanationPos();
	SimpleMath::Vector2 MiddleExplanationPos();
	SimpleMath::Vector2 LeftExplanationPos();

private:

	void ExplanationRender(SimpleMath::Vector2 pos, int type);

	bool m_mouseSelectButton[3];
	int m_prvMouseValue;

};