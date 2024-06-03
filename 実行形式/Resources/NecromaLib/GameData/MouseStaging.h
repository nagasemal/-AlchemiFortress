//--------------------------------------------------------------------------------------
// File: MouseStaging.h
//
//　マウスの挙動に合わせてパーティクルを発生させるクラス
// 
// Author: KazumaNagase
//--------------------------------------------------------------------------------------

#pragma once

class Particle_2D;

class MouseStaging
{
public:
	MouseStaging();
	~MouseStaging();

	void Update();
	void Render();

private:

	std::unique_ptr<Particle_2D> m_clickParticle;
	std::unique_ptr<Particle_2D> m_holdParticle;
	std::unique_ptr<Particle_2D> m_releaseParticle;

	std::unique_ptr<Particle_2D> m_stopParticle;

};