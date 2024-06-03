#include "pch.h"
#include "MachineDraw.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/Singleton/ShareJsonData.h"

// 円周の一番小さい場合におけるマシンの最大数
#define CIRCLE_MAX_MIN  4

// 円周の最大ライン
#define CIRCLE_MAX_LINE 6

// ライン同士の間隔
#define CIRCLE_LINE_DISTANCE 5.5f

MachineDraw::MachineDraw()
{
}

MachineDraw::~MachineDraw()
{
}

void MachineDraw::Initialize()
{

	m_AMFilter = std::make_unique<AlchemicalMachineFilter>();

	SettingAMMachine_Title();

}

void MachineDraw::Initialize(int number)
{
	m_AMFilter = std::make_unique<AlchemicalMachineFilter>();

	SettingAMMachine_Select(number);

}

void MachineDraw::Update()
{
	for (int i = 0; i < m_AMobjects.size(); i++)
	{

		MovingMachine(i);

	}
}

void MachineDraw::Render()
{
	for (int i = 0; i < m_AMobjects.size(); i++)
	{
		//// Noneならば飛ばす
		//if (m_AMobjects[i]->GetModelID() == NONE) return;

		// モデルの描画			オブジェクトに割り当てられたIDをもとにモデル配列からデータを取り出す
		m_AMobjects[i]->ModelRender(
			m_AMFilter->HandOverAMModel(m_AMobjects[i]->GetModelID()),
			m_AMFilter->GetRingModel(m_AMobjects[i]->GetModelID()));

		m_AMobjects[i]->Draw();
	}
}

void MachineDraw::Finalize()
{
}

void MachineDraw::MovingMachine(int number)
{

	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	// 0,0,0を中心に回転移動
	DirectX::SimpleMath::Matrix matrix = DirectX::SimpleMath::Matrix::Identity;

	matrix *= DirectX::SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(0.1f)) * deltaTime;

	// 回転後の座標を代入
  	m_AMobjects[number]->SetPos(DirectX::SimpleMath::Vector3::Transform(m_AMobjects[number]->GetPos(), matrix));

	m_AMobjects[number]->SetColor(DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));

}

void MachineDraw::SettingAMMachine_Title()
{
	int counter = 0;

	for (int i = 1; i < CIRCLE_MAX_LINE; i++)
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

			m_AMobjects[counter]->SetColor(DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
			counter++;
		}
	}
}

void MachineDraw::SettingAMMachine_Select(int number)
{
	number;

	Stage_ClearData clearData = ShareJsonData::GetInstance().GetClearData();

	int counter = 0;

	m_AMobjects.clear();

	for (int i = 0; i < CIRCLE_MAX_LINE; i++)
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

			m_AMobjects[counter]->SetColor(DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));

			counter++;
		}
	}

}
