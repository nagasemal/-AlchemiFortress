#include "pch.h"
#include "Field.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "NecromaLib/Singleton/SpriteLoder.h"

#define POS_Y -1.5f

#define RAGE 120.f

#define RAGE_Y 1.0f

Field::Field():
	m_hitMouseFlag()
{
}

Field::~Field()
{
}

void Field::Initialize()
{
	m_data.pos		= SimpleMath::Vector3(0,POS_Y,0);
	m_data.rage		= SimpleMath::Vector3(RAGE, RAGE_Y,RAGE);
}

void Field::Update()
{
	m_hitMouseFlag = false;

	SimpleMath::Vector3 mouseWolrdPos = InputSupport::GetInstance().GetMousePosWolrd();

	Circle objectData = Circle(m_data.pos, RAGE * (RAGE_Y / 2.0f));

	//　====================[　フィールドToマウスポインター　]
	if (PointerToCircle(objectData,mouseWolrdPos)) m_hitMouseFlag = true;

}

void Field::Draw()
{
}

void Field::Render(DirectX::Model* model)
{

	ShareData& pSD = ShareData::GetInstance();

	//　====================[　位置や大きさの行列を生成　]
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);

	//　====================[　床を描画　]
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false,[&]()
		{

			ModelShader::GetInstance().ModelDrawShader(
				SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f),
				SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SpriteLoder::GetInstance().GetFiledTexture());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetFiledTexture().GetAddressOf());
			pSD.GetContext()->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetFiledNormalMap().GetAddressOf());

		});
}

void Field::WriteDepth(DirectX::Model* model)
{

	ShareData& pSD = ShareData::GetInstance();

	//　====================[　位置や大きさの行列を生成　]
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(m_data.rage);
	modelData *= SimpleMath::Matrix::CreateTranslation(m_data.pos.x, m_data.pos.y, m_data.pos.z);

	//　====================[　床を描画　]
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]()
		{

			ModelShader::GetInstance().ShadowModelDraw(false);

		});
}

void Field::Finalize()
{
}
