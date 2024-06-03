//--------------------------------------------------------------------------------------
// File		: MachineGauge.h
//
//　アルケミカルマシンのHP表示用UI　基本的な処理はGaugeクラスと同等
// 
// Use		: MachineExplanation
// Date		: 2023.6.12
// Author	: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "NecromaLib/GameData/UserInterfase.h"

class AlchemicalMachineObject;

class MachineGauge
{
public:
	MachineGauge();
	~MachineGauge();

	//// マシンのHPゲージ
	//void AddHPGauge(
	//	  SimpleMath::Vector2 position
	//	, SimpleMath::Vector2 scale
	//	, UserInterface::ANCHOR anchor);

	void Update(int hp,int maxHp);
	void Render(AlchemicalMachineObject* gameObject);

private:

	//// HP
	//std::unique_ptr<UserInterface> m_gauge_Hp;
	//std::unique_ptr<UserInterface> m_base_Hp;

};