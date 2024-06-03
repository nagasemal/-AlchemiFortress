#pragma once

#include "ICommand_UI.h"
#include <vector>

class Commander_UI
{
public:

	std::vector<ICommand_UI*> GetCommand() { return m_commands; }
	void AddCommand(ICommand_UI* command) { m_commands.push_back(command); }

public:

	Commander_UI();
	~Commander_UI();

	void Execute();
	void Execute(int index);

	void Reset();

private:

	std::vector<ICommand_UI*> m_commands;
};