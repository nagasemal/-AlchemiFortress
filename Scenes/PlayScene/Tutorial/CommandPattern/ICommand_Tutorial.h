#pragma once

class TutorialManager;

enum UI_TYPE : int
{
	UI_ALCHEMI,
	UI_GAUGE,
	UI_EXPLANATION
};

class ICommand_Tutorial
{
protected:

	bool m_initializeFlag = false;	// 初期化処理が終了したことを知らせるフラグ
	bool m_completion = false;		// コマンドが終了したことを知らせるフラグ

public:
	virtual void Reset() = 0;

	// 処理が開始した際に通す処理
	virtual void Initialize() = 0;
	// 処理実行関数
	virtual void Execute() = 0;
	// 処理が終了した際に通す処理
	virtual void Finalize() = 0;

	virtual ~ICommand_Tutorial() = default;

	virtual void SetTutorialManagerPtr(TutorialManager* object) = 0;

	bool GetCompletion() { return  m_completion; }

	void SetCompletion(bool flag) { m_completion = flag; }

};