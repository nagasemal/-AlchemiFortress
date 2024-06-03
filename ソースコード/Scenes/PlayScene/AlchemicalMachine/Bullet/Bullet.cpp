#include "pch.h"
#include "Bullet.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ModelShader.h"

Bullet::Bullet(float speed, float damage, float life,SimpleMath::Color color, SimpleMath::Vector3 pos, SimpleMath::Vector3 targetVector)
{
	m_bulletData.speed = speed;
	m_bulletData.damage = damage;
	m_bulletData.life = life;

	m_color = color;

	m_data.pos = pos;
	m_startPos = pos;
	m_data.rage = SimpleMath::Vector3(1, 1, 1);

	m_targetVectol = targetVector;

	m_enemyHit = false;

	m_time = 0.0f;

}

Bullet::Bullet(BulletData data, SimpleMath::Color color, SimpleMath::Vector3 pos, SimpleMath::Vector3 targetVector)
{
	m_bulletData = data;

	m_color = color;

	m_data.pos = pos;
	m_startPos = pos;
	m_data.rage = SimpleMath::Vector3(1, 1, 1);

	m_targetVectol = targetVector;

	m_enemyHit = false;

	m_time = 0.0f;
}

Bullet::~Bullet()
{

}

void Bullet::Initialize()
{

}

void Bullet::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_bulletData.life -= deltaTime;

	m_time += deltaTime;

	SimpleMath::Vector3 vec = m_targetVectol - m_startPos;

	vec.y = m_data.pos.y;

	// ³‹K‰»
	vec.Normalize();

	m_data.pos += vec * (deltaTime * m_bulletData.speed);

}

void Bullet::Draw()
{

}

void Bullet::Finalize()
{

}

void Bullet::Render(GeometricPrimitive* geo)
{

	ShareData& pSD = ShareData::GetInstance();

	SimpleMath::Matrix textBox = SimpleMath::Matrix::CreateTranslation(m_data.pos.x, 1 , m_data.pos.z);

	geo->Draw(textBox, pSD.GetView(), pSD.GetProjection(), m_color, nullptr, false, [&] 
		{
		
			ModelShader::GetInstance().ModelDrawShader(m_color, SimpleMath::Vector4(m_time, 0.0f, 0.0f, 1.0f), SpriteLoder::GetInstance().GetRule());

			pSD.GetContext()->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetNormalMap(0).GetAddressOf());
		
		});

}

bool Bullet::deleteRequest()
{
	return m_bulletData.life <= 0;
}
