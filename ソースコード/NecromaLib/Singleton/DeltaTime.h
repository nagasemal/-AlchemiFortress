//--------------------------------------------------------------------------------------
// File: DeltaTime.h
//
// デルタタイムを得る為のシングルトンクラス
//
// Date: 2023.4.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "StepTimer.h"

class DeltaTime
{
public:
	~DeltaTime() = default;

	DeltaTime(const DeltaTime&) = delete;
	DeltaTime& operator=(const DeltaTime&) = delete;

	static void Create();
	static void Destroy();

	static DeltaTime& GetInstance() { return *instance; }

	/// <summary>
	/// ゲーム内で変化するデルタタイムを返します
	/// </summary>
	/// <returns></returns>
	float GetDeltaTime() { return ((float)m_stepTimer.GetElapsedSeconds() * m_doubleSpeed) * !m_stopTimeFlag; }

	/// <summary>
	/// 値の加算減算がおこなわれてない素のデルタタイムを返します
	/// </summary>
	/// <returns></returns>
	float GetNomalDeltaTime();

	/// <summary>
	/// ステップタイマークラスを返します
	/// </summary>
	DX::StepTimer GetStepTimer() { return m_stepTimer; }

	/// <summary>
	/// デルタタイムを入力
	/// </summary>
	/// <param name="time"></param>
	void SetDeltaTime(DX::StepTimer timer)		{ m_stepTimer = timer; }
	
	/// <summary>
	/// 倍速量を入力
	/// </summary>
	/// <param name="speed"></param>
	void SetDoubleSpeed(float speed)	{ m_doubleSpeed = speed; }

	/// <summary>
	/// ゲーム内時間を止めるフラグを入力
	/// </summary>
	/// <param name="flag"></param>
	void SetStopFlag(bool flag)			{ m_stopTimeFlag = flag; }

private:
	DeltaTime();
	static DeltaTime* instance;

	// 元のスピードよりやや早くする
	const float SPEED = 1.2f;

	// ステップタイマー
	DX::StepTimer m_stepTimer;

	// 倍速量
	float m_doubleSpeed;

	// 返すデルタタイムを0にする
	bool m_stopTimeFlag;

};