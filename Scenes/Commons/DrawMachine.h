//--------------------------------------------------------------------------------------
// File: MachineDraw.h
//
// 演出用にマシンを描画する
// 
// Date: 2023.8.10
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineFilter.h"

class DrawMachine
{
public:
	DrawMachine();
	~DrawMachine();

	void Initialize();
	void Initialize(int number);
	void Update();
	void Render();
	void Finalize();

private:

	// 円周上に並べる処理
	SimpleMath::Vector3 SetVelocityCircle(int index, int max, float range)
	{

		float radian = XM_2PI / static_cast<float>(max);

		// 一つの角度が分かればもう全部わかる
		float x = range * cosf(radian * index);
		float z = range * sinf(radian * index);

		SimpleMath::Vector3 pos = { x,0,z };

		return pos;
	}

	void SettingAMMachine_Title();
	void SettingAMMachine_Select(int number);

	void MovingMachine(int index);

	std::unique_ptr<AlchemicalMachineFilter>				m_AMFilter;
	std::vector<std::shared_ptr<AlchemicalMachineObject>>	m_AMobjects;

};