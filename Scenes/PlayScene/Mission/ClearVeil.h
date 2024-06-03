#pragma once
#include "NecromaLib/GameData/Animation.h"

class UserInterface;
class Particle_2D;

class ClearVeil
{
public:
	ClearVeil(int waveNum);
	~ClearVeil();

	void Update(bool startFlag);
	void Render(int nowWave);

	// 次のWaveに移行させる
	bool NextWave();

private:

	// NextWaveの文字描画
	std::unique_ptr<UserInterface> m_nextWave_Top,m_nextWave_Bottom;
	// WAVEクリアした際に流すアニメーション用変数
	AnimationData m_waveAnimation_first;
	AnimationData m_waveAnimation_second;

	// Wave数を視覚的に表現するアニメーション用変数
	std::vector<AnimationData> m_waveAnimation_waves;

};