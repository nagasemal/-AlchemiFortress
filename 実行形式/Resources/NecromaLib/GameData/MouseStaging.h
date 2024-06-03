//--------------------------------------------------------------------------------------
// File: MouseStaging.h
//
//�@�}�E�X�̋����ɍ��킹�ăp�[�e�B�N���𔭐�������N���X
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