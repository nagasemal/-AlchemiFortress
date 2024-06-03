#pragma once

#include "NecromaLib/GameData/Animation.h"

class ResultDirection
{
public:
	ResultDirection();
	~ResultDirection();

	void Update();

	void Render();

private:

	AnimationData m_animationData;

	bool m_rotateNowFlag;

};