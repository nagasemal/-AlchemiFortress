#include "pch.h"
#include "Commander_UI.h"
#include "Scenes/Commons/SelectionUI.h"

Commander_UI::Commander_UI()
{
}

Commander_UI::~Commander_UI()
{
}

void Commander_UI::Execute()
{

	for (auto& commands : m_commands)
	{
		commands->Execute();
	}

}

void Commander_UI::Execute(int index)
{

	m_commands[index]->Execute();

}

void Commander_UI::Reset()
{
	m_commands.clear();
}
