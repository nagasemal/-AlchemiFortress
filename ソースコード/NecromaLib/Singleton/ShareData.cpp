#include <pch.h>
#include "ShareData.h"
#include "ShareJsonData.h"

#include "NecromaLib/GameData/BinaryFile.h"

ShareData* ShareData::instance = nullptr;

ShareData::ShareData():
	m_commonStates(),
	m_deviceResources(),
	m_spriteBatch(),
	m_camera(),
	m_effectFactory()
{
}

void ShareData::Create()
{
	if (instance == nullptr)
	{
		instance = new ShareData;
	}
}

void ShareData::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}