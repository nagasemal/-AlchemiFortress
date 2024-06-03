#include "pch.h"
#include "MousePointer.h"
#include <WICTextureLoader.h> 
#include "NecromaLib/GameData/BinaryFile.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "Scenes/PlayScene/Shadow/Particle.h"

#define POINTER_RAGE			SimpleMath::Vector3(0.5f, 0.5f, 0.5f)
#define POINTER_RAGE_BIG		SimpleMath::Vector3(2.5f, 2.5f, 2.5f)

#define PARTICLE_SHIFT			2.0f

MousePointer::MousePointer():
	m_time()
{
}

MousePointer::~MousePointer()
{
}

void MousePointer::Initialize()
{
	//　初期化　　オブジェクト配置時は当たり判定を大きくする
	m_data.pos = SimpleMath::Vector3::Zero;
	m_data.rage = POINTER_RAGE;

	ShareData& pSD = ShareData::GetInstance();
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_mouseCursorModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/MouseCursor.cmo", *fx);

	m_particle = std::make_unique<Particle>(Particle::EFFECT_TYPE::CLICK);
	m_particle->Initialize();

}

void MousePointer::Update()
{

	InputSupport& pINP = InputSupport::GetInstance();
	auto mouse = InputSupport::GetInstance().GetMouseState();

	m_time += DeltaTime::GetInstance().GetDeltaTime();

	if (m_time >= 1) m_time = 1.0f;

	m_data.pos = pINP.GetMousePosWolrd();
	m_data.pos.y = -1.0f;
	m_data.rage = POINTER_RAGE;

	// パーティクルの更新
	m_particle->UpdateParticle();

	// 左クリックされたらパーティクルを出現させる
	if (pINP.LeftButton_Press())
	{
		m_particle->OnShot(SimpleMath::Vector3(m_data.pos.x, m_data.pos.y + PARTICLE_SHIFT, m_data.pos.z), true, SimpleMath::Color(0.8, 0.8, 0.8, 1.0f));
	}

}

void MousePointer::Draw()
{

	m_particle->Render();
	ModelDraw(m_mouseCursorModel.get());
}

void MousePointer::ModelDraw(DirectX::Model* model)
{

	// モデル情報(位置,大きさ)
	SimpleMath::Matrix modelData = SimpleMath::Matrix::Identity;
	modelData = SimpleMath::Matrix::CreateScale(POINTER_RAGE);

	modelData *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(m_data.pos.x, m_data.pos.y + 2.0f, m_data.pos.z));

	ShareData& pSD = ShareData::GetInstance();

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), modelData, pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			ModelShader::GetInstance().MachineDrawShader(SimpleMath::Color(1.0f,1.0f,1.0f,0.5f),
													   SimpleMath::Vector4(m_time,0.6f,0.0f,1.0f),
													   SpriteLoder::GetInstance().GetRule());

			ModelShader::GetInstance().ToransparentShader();

		});

	//シェーダの登録を解除しておく
	pSD.GetContext()->VSSetShader(nullptr, nullptr, 0);
	pSD.GetContext()->PSSetShader(nullptr, nullptr, 0);

}

void MousePointer::Finalize()
{
}

void MousePointer::HitMachine(SimpleMath::Vector3 pos)
{
	m_data.pos = pos;
	m_data.rage = POINTER_RAGE_BIG;
}

void MousePointer::ObjectDragMode()
{

	m_data.rage = POINTER_RAGE_BIG;

}

void MousePointer::ReleaseLeftButtom()
{

	m_data.rage = POINTER_RAGE;

}
