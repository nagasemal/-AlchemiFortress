#pragma once
//--------------------------------------------------------------------------------------
// File: SoundData.h
//
// BGMやSEを流す為のクラス
// 
//
// Date: 2023.9.13
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#include "NecromaLib/GameData/Audio/BGMs.h"
#include "NecromaLib/GameData/Audio/SEs.h"
#include "NecromaLib/GameData/Audio/SystemSE.h"

#include "NecromaLib/GameData/Animation.h"

class SoundData
{

public:
	~SoundData() = default;

	SoundData(const SoundData&) = delete;
	SoundData& operator=(const SoundData&) = delete;

	static void Create();
	static void Destroy();

	static SoundData& GetInstance() { return *instance; }

	void SoundLoad(DirectX::AudioEngine* audioEngine);

	// BGMを流します
	void PlayBGM(XACT_WAVEBANK_BGMS name, bool stop);
	// 全てのBGMを止めます
	void StopBGM();

	// SEを再生
	void PlaySE(XACT_WAVEBANK_SES name);

	// SEを止める
	void StopSE(XACT_WAVEBANK_SES name);

	// システムSEを再生
	void PlaySystemSE(XACT_WAVEBANK_SYSTEMSE name);

	// システムSEを再生(現在鳴っているシステムSEを止め、新しく流す)
	void PlaySystemSE_Auto(XACT_WAVEBANK_SYSTEMSE name);

	// システムSEの停止
	void StopSystemSE(XACT_WAVEBANK_SYSTEMSE name);

	// BGMの音量設定
	void SetBGMVol(float vol, float max)		{ m_bgmVol = vol; m_bgmVolMax = max; }

	// SEの音量設定
	void SetSEVol(float vol, float max)			{ m_seVol = vol; m_seVolMax = max; }

	// システムSEの音量設定
	void SetSystemSEVol(float vol, float max)	{ m_systemVol = vol; m_systemVolMax = max;}

private:
	SoundData();
	static SoundData* instance;

	std::unique_ptr<DirectX::WaveBank> m_BGMBank;
	std::unique_ptr<SoundEffectInstance> m_BGMInstance[XACT_WAVEBANK_BGMS_ENTRY_COUNT];

	std::unique_ptr<DirectX::WaveBank> m_SEBank;
	std::unique_ptr<SoundEffectInstance> m_SEInstance[XACT_WAVEBANK_SES_ENTRY_COUNT];

	std::unique_ptr<DirectX::WaveBank> m_SystemSEBank;
	std::unique_ptr<SoundEffectInstance> m_SystemSEInstance[XACT_WAVEBANK_SYSTEMSE_ENTRY_COUNT];


	float m_bgmVol,m_bgmVolMax;			// BGMのボリューム
	float m_seVol, m_seVolMax;			// SE のボリューム
	float m_systemVol, m_systemVolMax;	// Systemのボリューム(クリック音等)
	AnimationData m_timer;

};