//--------------------------------------------------------------------------------------
// File: InputSupport.h
//
//　マウス入力,キーボード入力をサポートするシングルトンクラス
// 
// Date: 2023.05.09
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
class InputSupport
{
private:

	InputSupport();
	static InputSupport* instance;

	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	DirectX::Mouse::ButtonStateTracker m_mouseTracker;

	//ワールド空間座標上のマウス
	SimpleMath::Vector3 m_WorldScreenMouse;

	// UIに触れると、設定されているレイヤーの番号が入る
	int m_nowLayer;

	bool m_hitUIFlag;

	// キー表示を見れるようにする
	bool m_keyDisplayMode;

public:
	~InputSupport() = default;

	InputSupport(const InputSupport&) = delete;
	InputSupport& operator=(const InputSupport&) = delete;

	static void Create();
	static void Destroy();

	static InputSupport& GetInstance() { return *instance; }

	void Update();

	/// <summary>
	/// 現在触れているUIレイヤーを設定します
	/// </summary>
	/// <param name="layer">UIレイヤー番号</param>
	void SetLayer(int layer);

	/// <summary>
	/// 現在触れているUIレイヤー番号を返します
	/// </summary>
	/// <returns></returns>
	bool GetLayer() { return m_nowLayer; }

	/// <summary>
	/// UIに触れている
	/// </summary>
	void HitUI() { m_hitUIFlag = true; }

	/// <summary>
	/// UIに触れていない
	/// </summary>
	void ExitUI() { m_hitUIFlag = false; }

	/// <summary>
	/// UIへの接触判定
	/// </summary>
	/// <returns></returns>
	bool GetHitUI() {return m_hitUIFlag;}

	/// <summary>
	/// キーボードクラスを設定
	/// </summary>
	/// <param name="tracker">生成したトラッカー</param>
	void SetKeybordState(DirectX::Keyboard::KeyboardStateTracker tracker)	{m_keyboardTracker = tracker; }

	/// <summary>
	/// キーボードトラッカーを返す
	/// </summary>
	/// <returns></returns>
	DirectX::Keyboard::KeyboardStateTracker GetKeybordState()				{ return m_keyboardTracker; }

	/// <summary>
	/// マウスクラスを設定
	/// </summary>
	/// <param name="tracker">生成したマウストラッカー</param>
	void SetMouseState(DirectX::Mouse::ButtonStateTracker tracker) { m_mouseTracker = tracker; }

	/// <summary>
	/// マウストラッカーを返す
	/// </summary>
	/// <returns></returns>
	DirectX::Mouse::ButtonStateTracker GetMouseState() { return m_mouseTracker; }

public:

	// マウス左が押された瞬間
	bool LeftButton_Press();
	// マウス左が離された瞬間
	bool LeftButton_Release();
	// マウス左が押され続けている
	bool LeftButton_Held();

	// マウス右が押された瞬間
	bool RightButton_Press();
	// マウス右が離された瞬間
	bool RightButton_Release();
	// マウス右が押され続けている
	bool RightButton_Held();

	// マウスホイールが押された瞬間
	bool MiddleButton_Press();
	// マウスホイールが離された瞬間
	bool MiddleButton_Release();
	// マウスホイールが押され続けている
	bool MiddleButton_Held();

	// キー表示モードになっている
	bool GetKeyDisplayMode();

	// マウス座標(ワールド空間)
	SimpleMath::Vector3 GetMousePosWolrd();

	// マウス座標(スクリーン空間)
	SimpleMath::Vector2 GetMousePosScreen();

};