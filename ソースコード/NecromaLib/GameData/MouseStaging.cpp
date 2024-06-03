#include "pch.h"
#include "MouseStaging.h"

#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareData.h"

#include "NecromaLib/GameData/Particle_2D.h"

MouseStaging::MouseStaging()
{
	m_clickParticle			= std::make_unique<Particle_2D>();
	m_holdParticle			= std::make_unique<Particle_2D>();
	m_releaseParticle		= std::make_unique<Particle_2D>();

	m_stopParticle			= std::make_unique<Particle_2D>();
}

MouseStaging::~MouseStaging()
{
}

void MouseStaging::Update()
{
	InputSupport& pINP = InputSupport::GetInstance();

	m_clickParticle			->OnShot("MouseClick", pINP.GetMousePosScreen(), pINP.LeftButton_Press());
	m_holdParticle			->Update("MouseHold", pINP.GetMousePosScreen(), pINP.LeftButton_Held());
	m_releaseParticle		->OnShot("MouseRelease", pINP.GetMousePosScreen(), pINP.LeftButton_Release());
	m_stopParticle			->OnShot("ClearParticle", pINP.GetMousePosScreen(), pINP.RightButton_Press());

	m_clickParticle			->UpdateParticle();
	m_holdParticle			->UpdateParticle();
	m_releaseParticle		->UpdateParticle();
	m_stopParticle			->UpdateParticle();

}

void MouseStaging::Render()
{

	m_clickParticle			->Render();
	m_holdParticle			->Render();
	m_releaseParticle		->Render();
	m_stopParticle			->Render();

	////　====================[　パーティクルの描画　]
	//SpriteLoder& pSL = SpriteLoder::GetInstance();
	////SpriteLoder::TextureData particleTex = pSL.GetUIArrow();
	//m_stopParticle->Render(pSL.GetAlphabet().tex, RECT{ 0,0, 36, 36});

}
