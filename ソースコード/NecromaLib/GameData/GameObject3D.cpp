#include "pch.h"
#include "GameObject3D.h"

GameObjct3D::GameObjct3D() :
	m_data(),
    m_colliderActive(true)
{
}

GameObjct3D::~GameObjct3D()
{
}

int GameObjct3D::HitObject_Box(GameObjct3D* data)
{
    // “–‚½‚è”»’è‚Ìˆ—‚ð’Ê‚³‚È‚¢
	if (m_colliderActive && data->GetColliderActive())
	{

		return	AABB_3D(-data->GetRage() / 2 + data->GetPos(),
			data->GetRage() / 2 + data->GetPos(),
			-m_data.rage / 2 + m_data.pos,
			m_data.rage / 2 + m_data.pos);
	}

	return false;

}

bool GameObjct3D::HitObject_Circle(GameObjct3D* data)
{
	if (!m_colliderActive) return false;

	return CircleCollider(GetCircle(), data->GetCircle());
}
