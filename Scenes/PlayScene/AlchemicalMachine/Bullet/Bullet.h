#pragma once
#include "NecromaLib/GameData/GameObject3D.h"
#include "Scenes/PlayScene/Enemy/EnemyObject.h"

class EnemyObject;

class Bullet : public GameObjct3D
{
public:

	struct BulletData
	{
		float speed;
		float damage;
		float life;
	};

public:
	Bullet(float speed, float damage, float life,SimpleMath::Color color, SimpleMath::Vector3 pos, SimpleMath::Vector3 targetVector);
	Bullet(BulletData data, SimpleMath::Color color, SimpleMath::Vector3 pos, SimpleMath::Vector3 targetVector);
	~Bullet();

	// 初期化
	void Initialize()					override;

	// 更新
	void Update()						override;

	// 描画　(UI関係)
	void Draw()							override;

	// 終了処理
	void Finalize()						override;

	void Render(GeometricPrimitive* geo);

	// 親クラスに消す処理を行わせる
	bool deleteRequest();

public:

	void SetLife(float life)	{ m_bulletData.life = life; }
	void SetEnemyHit(bool flag) { m_enemyHit = flag; }

	float GetDamage()			{ return m_bulletData.damage;}
	float GetLife()				{ return m_bulletData.life; }
	bool GetEnemyHit()			{ return m_enemyHit; }
	SimpleMath::Color GetColor()	{ return m_color; }

private:

	BulletData  m_bulletData;

	SimpleMath::Color m_color;
	SimpleMath::Vector3 m_startPos;
	SimpleMath::Vector3 m_targetVectol;

	float m_time;
	bool m_enemyHit;
};