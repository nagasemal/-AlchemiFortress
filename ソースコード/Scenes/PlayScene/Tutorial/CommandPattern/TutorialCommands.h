#pragma once

#include "ICommand_Tutorial.h"
#include "NecromaLib/GameData/CommonStruct.h"

/// <summary>
/// 何もしないコマンド
/// </summary>
class Tutorial_None : public ICommand_Tutorial
{
public:
	Tutorial_None();
	~Tutorial_None();

	// 処理が開始した際に通す処理
	void Initialize() override;
	// 処理実行関数
	void Execute() override;
	// 処理が終了した際に通す処理
	void Finalize() override;
	// 初期化の内容を記述
	void Reset() override;

	void SetTutorialManagerPtr(TutorialManager* object) override { m_tutorialManager = object; }

private:

	TutorialManager* m_tutorialManager;	// 対象のポインター
};

/// <summary>
/// マシンUIのチュートリアルコマンド
/// </summary>
class Tutorial_MachineUI : public ICommand_Tutorial
{
public:
	/// <summary>
	/// 渡されたマシンタイプのUIを先頭にします
	/// </summary>
	/// <param name="machineType">先頭にするマシンタイプ</param>
	/// <param name="selectType">flase : 選択しなくても良い		true : 選択で次のチュートリアルへ向かう</param>
	Tutorial_MachineUI(MACHINE_TYPE machineType,bool selectFlag);
	~Tutorial_MachineUI();

	// 処理が開始した際に通す処理
	void Initialize() override;
	// 処理実行関数
	void Execute() override;
	// 処理が終了した際に通す処理
	void Finalize() override;
	// 初期化の内容を記述
	void Reset() override;

	void SetTutorialManagerPtr(TutorialManager* object) override { m_tutorialManager = object; }

private:

	TutorialManager* m_tutorialManager;	// 対象のポインター

	MACHINE_TYPE m_machineType;			// 対応するマシンタイプ

	bool m_selectFlag;

};

/// <summary>
/// 錬金ボタンのチュートリアルコマンド
/// </summary>
class Tutorial_AlchemiUI : public ICommand_Tutorial
{
public:
	Tutorial_AlchemiUI(MACHINE_TYPE machineType,int maxCount);
	~Tutorial_AlchemiUI();

	// 処理が開始した際に通す処理
	void Initialize() override;
	// 処理実行関数
	void Execute() override;
	// 処理が終了した際に通す処理
	void Finalize() override;
	// 初期化の内容を記述
	void Reset() override;

	void SetTutorialManagerPtr(TutorialManager* object) override { m_tutorialManager = object; }

private:

	TutorialManager* m_tutorialManager;	// 対象のポインター

	MACHINE_TYPE m_machineType;			// 対応するマシンタイプ

	// 錬金に成功した回数
	int m_alchemiCount;

};

/// <summary>
/// エネミーの魔法陣を指す
/// </summary>
class Tutorial_EnemySpawn : public ICommand_Tutorial
{
public:
	/// <summary>
	/// エネミーの魔法陣に注視点を合わせる
	/// </summary>
	/// <param name="time"></param>
	Tutorial_EnemySpawn(float time);
	~Tutorial_EnemySpawn();

	// 処理が開始した際に通す処理
	void Initialize() override;
	// 処理実行関数
	void Execute() override;
	// 処理が終了した際に通す処理
	void Finalize() override;
	// 初期化の内容を記述
	void Reset() override;

	void SetTutorialManagerPtr(TutorialManager* object) override { m_tutorialManager = object; }

private:

	TutorialManager* m_tutorialManager;	// 対象のポインター

	float m_maxTime;

};

/// <summary>
/// 指定マシンを壊す(耐久値を0にする)
/// </summary>
class Tutorial_MachineBreak : public ICommand_Tutorial
{
public:
	/// <summary>
	/// 壊すマシンを決定する
	/// </summary>
	/// <param name="machineType">壊すマシンタイプ</param>
	Tutorial_MachineBreak(MACHINE_TYPE machineType);
	~Tutorial_MachineBreak();

	// 処理が開始した際に通す処理
	void Initialize() override;
	// 処理実行関数
	void Execute() override;
	// 処理が終了した際に通す処理
	void Finalize() override;
	// 初期化の内容を記述
	void Reset() override;

	void SetTutorialManagerPtr(TutorialManager* object) override { m_tutorialManager = object; }

private:

	TutorialManager* m_tutorialManager;	// 対象のポインター

	MACHINE_TYPE m_machineType;			// 対応するマシンタイプ

};

/// <summary>
/// 指定マシンを修繕する(耐久値をMAXにする)
/// </summary>
class Tutorial_MachineRepia : public ICommand_Tutorial
{
public:
	/// <summary>
	/// 治すマシンを決定する
	/// </summary>
	/// <param name="machineType">治すマシンタイプ</param>
	Tutorial_MachineRepia(MACHINE_TYPE machineType);
	~Tutorial_MachineRepia();

	// 処理が開始した際に通す処理
	void Initialize() override;
	// 処理実行関数
	void Execute() override;
	// 処理が終了した際に通す処理
	void Finalize() override;
	// 初期化の内容を記述
	void Reset() override;

	void SetTutorialManagerPtr(TutorialManager* object) override { m_tutorialManager = object; }

private:

	TutorialManager* m_tutorialManager;	// 対象のポインター

	MACHINE_TYPE m_machineType;			// 対応するマシンタイプ

};

/// <summary>
/// 置く場所をプログラム側で決めるコマンド_Upper用
/// </summary>
class Tutorial_UppperSelect : public ICommand_Tutorial
{
public:
	/// <summary>
	/// アタッカーマシンをアッパーマシンの範囲に収める適切な位置を教える
	/// </summary>
	Tutorial_UppperSelect(int spawnNum);
	~Tutorial_UppperSelect();

	// 処理が開始した際に通す処理
	void Initialize() override;
	// 処理実行関数
	void Execute() override;
	// 処理が終了した際に通す処理
	void Finalize() override;
	// 初期化の内容を記述
	void Reset() override;

	void SetTutorialManagerPtr(TutorialManager* object) override { m_tutorialManager = object; }

private:

	const float PARTICLE_TIME = 2.0f;

	TutorialManager* m_tutorialManager;					// 対象のポインター

	// パーティクルを出現させる時間
	float m_particleTime;

	int m_spawnNum;

};

/// <summary>
/// 置く場所をプログラム側で決めるコマンド_全部
/// </summary>
class Tutorial_NoneSelect : public ICommand_Tutorial
{
public:
	/// <summary>
	/// マシンを設置する箇所をパーティクルで知らせる
	/// </summary>
	Tutorial_NoneSelect(int spawnNum);
	~Tutorial_NoneSelect();

	// 処理が開始した際に通す処理
	void Initialize() override;
	// 処理実行関数
	void Execute() override;
	// 処理が終了した際に通す処理
	void Finalize() override;
	// 初期化の内容を記述
	void Reset() override;

	void SetTutorialManagerPtr(TutorialManager* object) override { m_tutorialManager = object; }

private:

	const float PARTICLE_TIME = 2.0f;

	// 対象のポインター
	TutorialManager* m_tutorialManager;

	// パーティクルを出現させる時間
	float m_particleTime;

	// 召喚する数
	int m_spawnNum;

};

/// <summary>
/// マシンUIの移動ロックを行う
/// </summary>
class Tutorial_LockMachineUI : public ICommand_Tutorial
{
public:
	/// <summary>
	/// マシンの設置ラインを決める
	/// </summary>
	Tutorial_LockMachineUI(bool activeFlag);
	~Tutorial_LockMachineUI();

	// 処理が開始した際に通す処理
	void Initialize() override;
	// 処理実行関数
	void Execute() override;
	// 処理が終了した際に通す処理
	void Finalize() override;
	// 初期化の内容を記述
	void Reset() override;

	void SetTutorialManagerPtr(TutorialManager* object) override { m_tutorialManager = object; }

private:

	// 対象のポインター
	TutorialManager* m_tutorialManager;

	bool m_activeFlag;

};

/// <summary>
/// 錬金ボタンの錬金ロックを行う
/// </summary>
class Tutorial_LockAlchemi : public ICommand_Tutorial
{
public:
	/// <summary>
	/// 錬金ボタンのロックを行う
	/// </summary>
	Tutorial_LockAlchemi(bool activeFlag);
	~Tutorial_LockAlchemi();

	// 処理が開始した際に通す処理
	void Initialize() override;
	// 処理実行関数
	void Execute() override;
	// 処理が終了した際に通す処理
	void Finalize() override;
	// 初期化の内容を記述
	void Reset() override;

	void SetTutorialManagerPtr(TutorialManager* object) override { m_tutorialManager = object; }

private:

	// 対象のポインター
	TutorialManager* m_tutorialManager;

	bool m_activeFlag;

};

/// <summary>
/// チュートリアルのテキストを進める矢印を作ります
/// </summary>
class Tutorial_TextArrow : public ICommand_Tutorial
{
public:
	/// <summary>
	/// マシンの設置ラインを決める
	/// </summary>
	Tutorial_TextArrow(int type);
	~Tutorial_TextArrow();

	// 処理が開始した際に通す処理
	void Initialize() override;
	// 処理実行関数
	void Execute() override;
	// 処理が終了した際に通す処理
	void Finalize() override;
	// 初期化の内容を記述
	void Reset() override;

	void SetTutorialManagerPtr(TutorialManager* object) override { m_tutorialManager = object; }

private:

	// 対象のポインター
	TutorialManager* m_tutorialManager;

	// どのような処理を行うか
	int m_type;

};