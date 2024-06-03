#include "pch.h"
#include "TutorialState.h"

#include "Scenes/PlayScene/Tutorial/TutorialManager.h"
#include "Scenes/PlayScene/PlayScene.h"

/// <summary>
///  チュートリアルの待機ステート
/// </summary>
Tutorial_IdlingState::Tutorial_IdlingState()
{
}

void Tutorial_IdlingState::Update(TutorialManager* manager)
{
	manager;
}

void Tutorial_IdlingState::Render(TutorialManager* manager)
{
	manager;
}

/// <summary>
///  チュートリアルの遷移ステート
/// </summary>
Tutorial_TransitionState::Tutorial_TransitionState()
{
}

void Tutorial_TransitionState::Update(TutorialManager* manager)
{
	manager;
}

void Tutorial_TransitionState::Render(TutorialManager* manager)
{
	manager;
}



/// <summary>
///  チュートリアルの終了ステート
/// </summary>
Tutorial_FinState::Tutorial_FinState()
{
}

void Tutorial_FinState::Update(TutorialManager* manager)
{
	manager;
}

void Tutorial_FinState::Render(TutorialManager* manager)
{
	manager;
}


/// <summary>
/// 特定マシンが壊れてしまうチュートリアルのステート
/// </summary>
Tutorial_BreakMachineState::Tutorial_BreakMachineState()
{
}

void Tutorial_BreakMachineState::Update(TutorialManager* manager)
{
	manager;
}

void Tutorial_BreakMachineState::Render(TutorialManager* manager)
{
	manager;
}

