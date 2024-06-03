//--------------------------------------------------------------------------------------
// File: EnemyCommander.h
//
//　エネミーコマンドのコマンダークラス
// 
// Date: 2023.10.26
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

#include "ICommand_Enemy.h"
#include <vector>

class EnemyCommander
{
public:

	std::vector<ICommand_Enemy*> GetCommand() { return m_commands; }
	void AddCommand(ICommand_Enemy* command);

public:

	EnemyCommander();
	~EnemyCommander();

	void Execute_ALL();	// 全て同時に回す
	void Execute_One(); // 配列順に回し、完了したものは回さない

	void Reset();

private:

	std::vector<ICommand_Enemy*> m_commands;

	int m_commandNumber;
};