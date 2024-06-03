#include "pch.h"
#include "DrawMachine.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/Singleton/ShareJsonData.h"

// 円周の一番小さい場合におけるマシンの最大数
#define CIRCLE_MAX_MIN  4

// 円周の最大ライン
#define CIRCLE_MAX_LINE 9

// ライン同士の間隔
#define CIRCLE_LINE_DISTANCE 5.5f

// マシン回転速度
#define ROTATION_SPEED 0.1f

DrawMachine::DrawMachine()
{
}

DrawMachine::~DrawMachine()
{
}

void DrawMachine::Initialize()
{
	m_AMFilter = std::make_unique<AlchemicalMachineFilter>();

	SettingAMMachine_Title();
}

void DrawMachine::Initialize(int number)
{
	m_AMFilter = std::make_unique<AlchemicalMachineFilter>();

	SettingAMMachine_Select(number);
}

void DrawMachine::Update()
{
	for (int i = 0; i < m_AMobjects.size(); i++)
	{

		MovingMachine(i);

	}
}

void DrawMachine::Render()
{
	for (int i = 0; i < m_AMobjects.size(); i++)
	{
		// Nonenマシンの描画を行わない
		if (m_AMobjects[i]->GetModelID() == MACHINE_TYPE::NONE) continue;

		// モデルの描画			オブジェクトに割り当てられたIDをもとにモデル配列からデータを取り出す
		m_AMobjects[i]->ModelRender(
			m_AMFilter->HandOverAMModel(m_AMobjects[i]->GetModelID()),
			m_AMFilter->GetRingModel(m_AMobjects[i]->GetModelID()));

		m_AMobjects[i]->Draw();
	}
}

void DrawMachine::Finalize()
{
}

void DrawMachine::MovingMachine(int number)
{

	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	// 0,0,0を中心に回転移動
	SimpleMath::Matrix matrix = SimpleMath::Matrix::Identity;
	matrix *= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(ROTATION_SPEED)) * deltaTime;

	// 回転後の座標を代入
  	m_AMobjects[number]->SetPos(SimpleMath::Vector3::Transform(m_AMobjects[number]->GetPos(), matrix));

	m_AMobjects[number]->Update_Common();

}

void DrawMachine::SettingAMMachine_Title()
{
	int counter = 0;
	int max = ShareJsonData::GetInstance().GetGameParameter().baseLV_MAX;

	for (int i = 1; i < max; i++)
	{
		for (int j = 0; j < CIRCLE_MAX_MIN * i; j++)
		{
			// マシンを追加
			m_AMobjects.push_back(m_AMFilter->HandOverAMClass(MACHINE_TYPE(i)));

			// 初期化処理
			m_AMobjects[counter]->Initialize();

			// 存在するライン決め
			m_AMobjects[counter]->SetLine(i);
			m_AMobjects[counter]->SummonAM(SetVelocityCircle(j, CIRCLE_MAX_MIN * i, i * CIRCLE_LINE_DISTANCE));

			counter++;
		}
	}
}

void DrawMachine::SettingAMMachine_Select(int number)
{
	number;

	Stage_ClearData clearData = ShareJsonData::GetInstance().GetClearData();

	int counter = 0;

	int max = ShareJsonData::GetInstance().GetGameParameter().baseLV_MAX;

	m_AMobjects.clear();

	for (int i = 0; i < max; i++)
	{
		for (int j = 0; j < CIRCLE_MAX_MIN * i; j++)
		{
			// マシンを追加
			m_AMobjects.push_back(m_AMFilter->HandOverAMClass(clearData.machines[counter].type));

			// 初期化処理
			m_AMobjects[counter]->Initialize();
			m_AMobjects[counter]->SetElement(clearData.machines[counter].element);

			// 存在するライン決め
			m_AMobjects[counter]->SetLine(i);
			m_AMobjects[counter]->SummonAM(SetVelocityCircle(j, CIRCLE_MAX_MIN * i, i * CIRCLE_LINE_DISTANCE));

			counter++;
		}
	}

}
