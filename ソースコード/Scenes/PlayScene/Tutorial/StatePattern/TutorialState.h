#pragma once
#include "ITutorialState.h"

class TutorialManager;
// 特定箇所が押されることを待機している状態
class Tutorial_IdlingState : public ITutorialState
{
public:
	Tutorial_IdlingState();
	~Tutorial_IdlingState() = default;

	void Update(TutorialManager* manager) override;
	void Render(TutorialManager* manager) override;

private:

};


// 特定箇所が押された後の移行状態
class Tutorial_TransitionState : public ITutorialState
{
public:
	Tutorial_TransitionState();
	~Tutorial_TransitionState() = default;

	void Update(TutorialManager* manager) override;
	void Render(TutorialManager* manager) override;

private:

};

// チュートリアルが終わった状態
class Tutorial_FinState : public ITutorialState
{
public:
	Tutorial_FinState();
	~Tutorial_FinState() = default;

	void Update(TutorialManager* manager) override;
	void Render(TutorialManager* manager) override;

private:

};

// マシンが壊れてしまった！状態を作るためのステート
class Tutorial_BreakMachineState : public ITutorialState
{
public:
	Tutorial_BreakMachineState();
	~Tutorial_BreakMachineState() = default;

	void Update(TutorialManager* manager) override;
	void Render(TutorialManager* manager) override;

private:

};