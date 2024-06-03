//--------------------------------------------------------------------------------------
// File: Commander_Tutorial.h
//
//　チュートリアルコマンドの実行クラス
// 
// Date: 2024.1.21
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include <vector>

class ICommand_Tutorial;

class TutorialCommander
{

public:
	enum TutorialType :int
	{
		NONE,						// チュートリアル処理がない
		ATTACKER_UI,				// マシンUIへの誘導を行う
		DEFENSER_UI,				// ディフェンサーマシンUIへの誘導を行う
		UPPER_UI,					// アッパーマシンUIへの誘導を行う
		RECOVERY_UI,				// リカバリーマシンUIへの誘導を行う
		MINING_UI,					// マイニングマシンUIへの誘導を行う

		ALCHEMI_UI,					// 錬金ボタンUIへの誘導を行う

		STOP_GAME_CLICK,			// ゲームの進行を止める(クリックで解除)
		ROTATION,					// 回転を止める,開始する
		INSTALLATION,				// マシンの新規設置を止める,開始する

		STAY_TIME,					// 指定秒数チュートリアルの更新処理を止める

		STOP_MACHINE,				// マシンの新規設置を不可にする
		START_MACHINE,				// マシンの新規設置を可能にする

		LR_BUTTON_LOCK,				// マシンUIの移動を制限する

		NONE_MACHINE_SELECT,		// ノーンマシンへの視線誘導を行う
		PROPHERAL_ATTACKER_MACHINE,	// アタッカーマシン周辺のマシンに視線誘導させる
		ENEMY_ATTENTION,			// エネミー出現位置に視線誘導を行う
		BREAK_MACHINE,				// マシンを破壊する
		REPAIR_MACHINE,				// マシンを修繕する

		TEXT_ARROW,					// チュートリアルのテキストを流す

		ALCHEMI_LOCK,				// 錬金ボタンをロック/解除する

		TRANSPARENT_START_UI,		// UI群を透明化
		TRANSPARENT_END_UI,			// UI群の透明化解除

	};

public:

	std::vector<ICommand_Tutorial*> GetCommand() { return m_commands; }

	// コマンドを追加する
	void AddCommand(ICommand_Tutorial* command, TutorialManager* object)
	{
		command->SetTutorialManagerPtr(object);
		m_commands.push_back(command);
	}

public:

	TutorialCommander() { m_commandNumber = 0; }
	~TutorialCommander() 
	{
	};

	// 全て同時に回す
	void Execute_ALL()
	{

		for (auto& command : m_commands)
		{
			command->Execute();
		}

	};	

	// 配列順に回し、完了したものは回さない
	void Execute_One()
	{
		int counter = 0;

		// 処理が完了したら次のコマンドへ移行する
		for (auto& command : m_commands)
		{
			counter += command->GetCompletion();

			// 終了処理を行う
			if (command->GetCompletion())
			{
				command->Finalize();
			}

		}

		if (counter >= m_commands.size()) return;

		m_commands[counter]->Initialize();
		m_commands[counter]->Execute();

	};

	void Reset()
	{
		for (auto& command : m_commands)
		{
			delete command;
		}
		m_commands.clear();
		m_commands.shrink_to_fit();
	};

private:

	std::vector<ICommand_Tutorial*> m_commands;

	int m_commandNumber;
};