#include <pch.h>
#include "DeltaTime.h"

DeltaTime* DeltaTime::instance = nullptr;

float DeltaTime::GetNomalDeltaTime()
{
	return (float)m_stepTimer.GetElapsedSeconds();
}

DeltaTime::DeltaTime():
	m_stopTimeFlag(false),
	m_doubleSpeed(1)
{

}

void DeltaTime::Create()
{
	if (instance == nullptr)
	{
		instance = new DeltaTime;
	}
}

void DeltaTime::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}