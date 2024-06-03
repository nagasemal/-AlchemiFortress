//--------------------------------------------------------------------------------------
// File: AlchemicalMachineFilter.h
//
//　受け取った引数に応じてモデルやクラス、内部ステータスを渡すクラス
// 
// Date: 2023.6.5
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

class AlchemicalMachineFilter
{
public:
	AlchemicalMachineFilter();
	~AlchemicalMachineFilter();

	/// <summary>
	/// マシンタイプに応じてモデルを渡す
	/// </summary>
	/// <param name="type">アルケミカルマシンのタイプ</param>
	/// <returns></returns>
	DirectX::Model* HandOverAMModel(MACHINE_TYPE type);

	/// <summary>
	/// マシンタイプに応じてAlchemicalMachineObjectを継承したクラスを渡す
	/// </summary>
	/// <param name="type">アルケミカルマシンのタイプ</param>
	/// <returns></returns>
	std::unique_ptr<AlchemicalMachineObject> HandOverAMClass(MACHINE_TYPE type);

	//// 本体とは別で動くリングモデルを渡します
	Model* GetRingModel(MACHINE_TYPE type);

	void Finalize();

private:

	std::unique_ptr<DirectX::Model> m_model[MACHINE_TYPE::NUM];

	std::unique_ptr<DirectX::Model> m_subModel[MACHINE_TYPE::NUM];

};