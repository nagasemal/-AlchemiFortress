#include "pch.h"
#include "SoundData.h"

SoundData* SoundData::instance = nullptr;

SoundData::SoundData() :
	m_bgmVol(),		m_bgmVolMax(0.5f),
	m_seVol(),		m_seVolMax(0.5f),
	m_systemVol(),	m_systemVolMax(0.5f),
	m_timer()
{

}

void SoundData::Create()
{
	if (instance == nullptr)
	{
		instance = new SoundData;
	}
}

void SoundData::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void SoundData::SoundLoad(DirectX::AudioEngine* audioEngine)
{
	// BGMの読み込み
	m_BGMBank = std::make_unique<WaveBank>(audioEngine, L"Resources/Audio/BGMs.xwb");
	for (int i = 0; i < XACT_WAVEBANK_BGMS_ENTRY_COUNT; i++)
	{
		m_BGMInstance[i] = m_BGMBank->CreateInstance(i);
	}

	// SEの読み込み
	m_SEBank = std::make_unique<WaveBank>(audioEngine, L"Resources/Audio/SEs.xwb");
	for (int i = 0; i < XACT_WAVEBANK_SES_ENTRY_COUNT; i++)
	{
		m_SEInstance[i] = m_SEBank->CreateInstance(i);
	}

	// SystemSEの読み込み
	m_SystemSEBank = std::make_unique<WaveBank>(audioEngine, L"Resources/Audio/SystemSE.xwb");
	for (int i = 0; i < XACT_WAVEBANK_SYSTEMSE_ENTRY_COUNT; i++)
	{
		m_SystemSEInstance[i] = m_SystemSEBank->CreateInstance(i);
	}

}

void SoundData::PlayBGM(XACT_WAVEBANK_BGMS name, bool stop)
{
	// フェードイン　アウト
	m_timer += 0.1f * (0 + !stop);

	m_bgmVol = m_timer;

	if (m_bgmVol >= m_bgmVolMax) m_bgmVol = m_bgmVolMax;
	if (m_bgmVol <= 0) m_bgmVol = 0;

	m_BGMInstance[name]->SetVolume(m_bgmVol);
	m_BGMInstance[name]->Play(true);

	if (stop && m_timer <= 0.0f)
	{
		m_BGMInstance[name]->Stop();
	}
}

void SoundData::StopBGM()
{
	//m_BGMInstance[0]->Stop();
	//m_BGMInstance[1]->Stop();
	for (int i = 0; i < XACT_WAVEBANK_BGMS_ENTRY_COUNT; i++)
	{
		m_BGMInstance[i]->Stop();
	}

}

void SoundData::PlaySE(XACT_WAVEBANK_SES name)
{
	m_SEInstance[name]->Stop();
	m_SEInstance[name]->SetVolume(m_seVolMax);
	m_SEInstance[name]->Play();

}

void SoundData::StopSE(XACT_WAVEBANK_SES name)
{
	m_SEInstance[name]->Stop();
}

void SoundData::PlaySystemSE_Auto(XACT_WAVEBANK_SYSTEMSE name)
{
	m_SystemSEInstance[name]->Stop();
	m_SystemSEInstance[name]->SetVolume(m_systemVolMax);
	m_SystemSEInstance[name]->Play();
}

void SoundData::StopSystemSE(XACT_WAVEBANK_SYSTEMSE name)
{
	m_SystemSEInstance[name]->Stop();
}

void SoundData::PlaySystemSE(XACT_WAVEBANK_SYSTEMSE name)
{
	m_SystemSEInstance[name]->SetVolume(m_systemVolMax);
	m_SystemSEInstance[name]->Play();
}
