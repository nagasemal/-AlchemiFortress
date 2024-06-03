#pragma once

#include "NecromaLib/GameData/GameObject2D.h"
#include "NecromaLib/GameData/Animation.h"

class DrawClick : public GameObject2D
{
public:
	DrawClick(SimpleMath::Vector2 pos, SimpleMath::Vector2 rage);
	~DrawClick();

	void Initialize()			override;
	// XVˆ—
	void Update()				override;
	// •`‰æ
	void Draw()					override;

	void Finalize()				override;

private:

	AnimationData m_animationData;

};