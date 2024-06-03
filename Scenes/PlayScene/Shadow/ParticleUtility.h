//--------------------------------------------------------------------------------------
// File: ParticleUtility.h
//
//  Particleクラスにて使用する値の遷移に使用　
//  
// Date: 2023.7.3
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once

class ParticleUtility
{
	//変数
private:


	//関数
private:
	//座標
	SimpleMath::Vector3 m_position;

	//速度
	SimpleMath::Vector3 m_velocity;
	//加速度
	SimpleMath::Vector3 m_accele;

	//スケール
	SimpleMath::Vector3 m_nowScale;
	SimpleMath::Vector3 m_startScale;
	SimpleMath::Vector3 m_endScale;

	//生存時間
	float m_life;
	float m_startLife;

	//ローテーション
	SimpleMath::Quaternion m_quaternion;
	SimpleMath::Vector3	m_rotate;

	//カラー
	SimpleMath::Color m_nowColor;
	SimpleMath::Color m_startColor;
	SimpleMath::Color m_endColor;

	//関数
public:
	/// <summary>
	/// 初期設定
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="pos">初期位置</param>
	/// <param name="velocity">速さ</param>
	/// <param name="accele">加速度</param>
	/// <param name="startScale">初期スケール</param>
	/// <param name="endScale">最終スケール</param>
	/// <param name="startColor">初期カラー</param>
	/// <param name="endColor">最終カラー</param>
	ParticleUtility(
		float life = 1.0f,
		SimpleMath::Vector3 pos = {0.0f,0.0f,0.0f},
		SimpleMath::Vector3 velocity = {0.0f,0.0f,0.0f},
		SimpleMath::Vector3 accele = {0.0f,0.0f,0.0f},
		SimpleMath::Vector3 startScale = {1.0f,1.0f,1.0f}, SimpleMath::Vector3 endScale = {0.0f,0.0f,0.0f},
		SimpleMath::Color startColor = {1.0f,1.0f,1.0f,1.0f}, SimpleMath::Color endColor = {0.0f,0.0f,0.0f,0.0f});
	//デストラクタ
	~ParticleUtility();

	// 更新
	bool Update();

	//getter

	// ポジション
	const SimpleMath::Vector3 GetPosition() { return m_position; }

	// 速さ
	const SimpleMath::Vector3 GetVelocity() { return m_velocity; }

	// 加速度
	const SimpleMath::Vector3 GetAccele() { return m_accele; }

	// 大きさ ： 現在
	const SimpleMath::Vector3 GetNowScale() { return m_nowScale; }
	// 大きさ ： 開始時
	const SimpleMath::Vector3 GetStartScale() { return m_startScale; }
	// 大きさ ： 終了時
	const SimpleMath::Vector3 GetEndScale() { return m_endScale; }

	// 生存時間 : 現在
	const float GetLife() { return m_life; }
	// 生存時間 : 開始時
	const float GetStartLife() { return m_startLife; }

	// 色 : 現在
	const SimpleMath::Color GetNowColor() { return m_nowColor; }
	// 色 : 開始時
	const SimpleMath::Color GetStartColor() { return m_startColor; }
	// 色 : 終了時
	const SimpleMath::Color GetEndColor() { return m_endColor; }

	// 回転クオータニオン
	const SimpleMath::Quaternion GetQuaternion() { return m_quaternion; }
	// 回転角
	const SimpleMath::Vector3 GetRad() { return m_rotate; }

	// setter

	// ポジション
	void SetPosition(SimpleMath::Vector3 pos) { m_position = pos; }
	// 速さ
	void SetVelocity(SimpleMath::Vector3 vel) { m_velocity = vel; }
	// 加速度
	void SetAccele(SimpleMath::Vector3 acc) { m_accele = acc; }

	// 大きさ ： 現在
	void SetNowScale(SimpleMath::Vector3 sca) { m_nowScale = sca; }
	// 大きさ ： 開始時
	void SetStartScale(SimpleMath::Vector3 sca) { m_startScale = sca; }
	// 大きさ ： 終了時
	void SetEndScale(SimpleMath::Vector3 sca) { m_endScale = sca; }

	// 生存時間 ： 現在
	void SetLife(float life) { m_life = life; }
	// 生存時間 ： 開始時
	void SetStartLife(float life) { m_startLife = life; }

	// 色 ： 現在
	void SetNowColor(SimpleMath::Color col) { m_nowColor = col; }
	// 色 ： 開始時
	void SetStartColor(SimpleMath::Color col) { m_startColor = col; }
	// 色 ： 終了時
	void SetEndColor(SimpleMath::Color col) { m_endColor = col; }

	// 回転角
	void SetRad(SimpleMath::Vector3 rad) { m_rotate = rad; }


};