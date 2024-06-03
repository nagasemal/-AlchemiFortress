#include "pch.h"
#include "Crystal.h"

#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"
#include "NecromaLib/Singleton/DeltaTime.h"

// モデルの大きさ
#define RAGE 0.8f

// 当たり判定の大きさ
#define COLLIDER_RAGE 1.2f

Crystal::Crystal(SimpleMath::Vector3 pos, float rotateY)
{
	m_data.pos = pos;
	m_data.rage = SimpleMath::Vector3(RAGE);
	m_rotateY = rotateY;

	m_type = CRYSTAL;

	m_selectFlag = false;

}

Crystal::~Crystal()
{
}

void Crystal::Initialize()
{
}

void Crystal::Update()
{
}

void Crystal::Draw()
{
}

void Crystal::Render(Model* model)
{

	ShareData& pSD = ShareData::GetInstance();

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= SimpleMath::Matrix::CreateRotationY(m_rotateY);
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);

;

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
		{

			ModelShader::GetInstance().ModelDrawShader(
				SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f),
				SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SpriteLoder::GetInstance().GetTowerPillarTexture());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetTowerPillarTexture().GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetTowerPillarTexture().GetAddressOf());

			//　====================[　深度ステンシルステートの設定　]
			pSD.GetContext()->OMSetDepthStencilState(pSD.GetCommonStates()->DepthDefault(), 0);

		});

}

void Crystal::WriteDepth(DirectX::Model* model)
{
	ShareData& pSD = ShareData::GetInstance();

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= SimpleMath::Matrix::CreateRotationY(m_rotateY);
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
		{

			ModelShader::GetInstance().ShadowModelDraw(false);

		});
}

bool Crystal::DeleteRequest()
{
	return false;
}

bool Crystal::GetHitMouse()
{
	SimpleMath::Vector3 mouseWolrdPos = InputSupport::GetInstance().GetMousePosWolrd();

	Circle mouseCircle(mouseWolrdPos, COLLIDER_RAGE);

	InputSupport& pINP = InputSupport::GetInstance();
	auto mouse = pINP.GetMouseState();
	bool leftRelease = pINP.LeftButton_Release();
	bool hitFlag = CircleCollider(GetCircle(), mouseCircle);

	if (leftRelease)
	{

		if (!hitFlag && m_selectFlag)
		{
			m_selectFlag = false;
		}
		else if (hitFlag && !m_selectFlag)
		{
			m_selectFlag = true;
		}
	}

	return 	m_selectFlag;
}

void Crystal::Finalize()
{
}
