#include "pch.h"
#include "EnemyCommander.h"

EnemyCommander::EnemyCommander():
	m_commandNumber(),
	m_commands()
{
}

EnemyCommander::~EnemyCommander()
{
}

void EnemyCommander::AddCommand(ICommand_Enemy* command)
{
	m_commands.push_back(command);
}

void EnemyCommander::Execute_ALL()
{
	// 全てのコマンドを実行
	for (auto& command : m_commands)
	{
		command->Execute();
	}
}

void EnemyCommander::Execute_One()
{
	// 現在のコマンド番号
	int counter = 0;

	// 稼働時間を満たしていたら加算
	for (auto& command : m_commands)
	{
		counter += command->GetCompletion();
	}

	// コマンド量より多くなった場合は0に戻す
	if (m_commands.size() <= counter)
	{
		// 全ての稼働完了済みコマンドをリセットする
		for (auto& command : m_commands)
		{
			command->SetCompletion(false);
			command->Reset();
		}

		counter = 0;
	}

	// 実行
	m_commands[counter]->Execute();

}

void EnemyCommander::Reset()
{
	m_commands.clear();
}
