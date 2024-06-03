//--------------------------------------------------------------------------------------
// File: Animation.h
//
// デルタタイムを得る為のシングルトンクラス
//
// Date: 2023.4.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "Easing.h"

struct AnimationData
{
	enum Type
	{
		STOP = 0,
		LOOP = 1,
	};

	// 時間
	float time;
	// 最小
	float min;
	// 最大
	float max;
	// 中間
	float middle;
	// 値の動き方
	Type type;
	// 何度繰り返しているか
	int iterations;
	// 実際の遷移値
	float anim;

	/// <summary>
	/// アニメーションに特化した構造体
	/// 上限下限中央値が存在するアニメーション処理なら有用
	/// 自動的にmin,maxの値でクランプ処理をします。
	/// </summary>
	/// <param name="time">開始時間</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <param name="middle">中央値</param>
	/// <param name="type">値を入れ続けた際の動き</param>
	AnimationData(float value = 0.0f, float min = 0.0f, float max = 1.0f, float middle = 0.5f, Type type = Type::STOP)
	{
		this->time = value;
		this->min = min;
		this->max = max;
		this->middle = middle;
		this->type = type;
		anim = 0;

		iterations = 0;
	}

	inline bool operator==(const AnimationData& value) const
	{
		return this->time == value.time;
	}

	inline bool operator!=(const AnimationData& value) const
	{
		return this->time != value.time;
	}

	inline AnimationData operator+(const AnimationData& value) const
	{
		return AnimationData(this->time + value.time,min,max);
	}

	inline AnimationData operator+(const float value) const
	{
		return AnimationData(this->time + value, min, max);
	}

	inline AnimationData operator-(const AnimationData& value) const
	{
		return AnimationData(this->time - value.time,min, max);
	}

	inline AnimationData operator*(const AnimationData& value) const
	{
		return AnimationData(this->time * value.time, min, max);
	}

	inline AnimationData operator*(const float value) const
	{
		return AnimationData(this->time * value, min, max);
	}

	inline AnimationData operator/(const AnimationData& value) const
	{
		return AnimationData(this->time / value.time, min, max);
	}

	inline AnimationData operator/(const float value) const
	{
		return AnimationData(this->time / value, min, max);
	}

	inline AnimationData& operator=(const AnimationData& value)
	{
		this->time = value.time;

		return *this;
	}


	inline operator float()
	{
		return time;
	}

	inline AnimationData& operator+=(const AnimationData& value)
	{

		this->time += value.time;

		if (MaxCheck())
		{
			this->time = max;
			iterations++;

			if (type == LOOP) 	this->time = min;
		}
		return *this;
	}

	inline AnimationData& operator-=(const AnimationData& value)
	{
		this->time -= value.time;

		if (MinCheck())
		{

			this->time = min;
			iterations++;

			if (type == LOOP) 	this->time = max;
		}

		return *this;
	}
	
	inline float Clamp()
	{
		return std::min(std::max(time, min), max);
	}

	// timeがmaxの値を越した
	inline bool MaxCheck()
	{
		return time >= max;
	}

	// timeがminの値を下回った
	inline bool MinCheck()
	{
		return time <= min;
	}

	// timeがmiddleを越した
	inline bool MiddleFlag()
	{
		return time >= middle;
	}

	inline bool ActiveFlag()
	{
		return !MaxCheck() && !MinCheck();
	}

};