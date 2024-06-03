#pragma once

#include "NecromaLib/GameData/GameObject2D.h"

class DrawKey;

class SelectionUI : public GameObject2D
{
public:
	SelectionUI();
	~SelectionUI();
	// 初期化
	void Initialize()			override;
	// 終了処理
	void Finalize()				override;

	// マウスが触れた処理
	bool HitMouse(bool layerCheck = false);
	// 左クリックされた処理
	bool ClickMouse();
	// 左クリックされて選択されている状態の処理
	bool SelectionMouse();
	// 左クリックが押され続けている処理
	bool HoldMouse();
	// マウスが離れた処理
	bool ExitMouse();

public:

	// 現在触れる状態か
	bool GetActiveFlag()				{ return m_activeFlag; }
	// 色情報を返す
	SimpleMath::Color GetColor()		{ return m_boxColor; }
	// 色情報(RGB)を返す
	SimpleMath::Vector3 GetColorRGB()	{ return SimpleMath::Vector3(m_boxColor.R(), m_boxColor.G(), m_boxColor.B()); }

	// 触れる状態/触れない状態への遷移
	void SetActiveFlag(bool flag)		{ m_activeFlag = flag; }

	// マウスが当たっている判定を変えられます
	void SetHitMouseFlag(bool flag)		{ m_hitMouseFlag = flag; }
	
	// 選択状態になっている判定を変えられます
	void SetSelectFlag(bool flag)		{ m_selectFlag = flag; }
	void SetKeySelectFlag(bool flag)	{ m_keySelectFlag = flag;}

	// 触れた際ワールド空間に影響を及ぼすか否か
	void SetWorldInfluenceFlag(bool flag) { m_worldInfluenceFlag = flag; }

	// 色を自由に変えられます。(ボックスを強調したい際などに使用)
	void SetColor(SimpleMath::Color color) { m_boxColor = color; }

	void SetRect(RECT rect) { m_rect = rect; }

	void SetLayer(int layer) { m_layer = layer; }

	// 初期位置情報と一緒に位置を上書きします
	void SetSavePos(SimpleMath::Vector2 pos) { m_saveData.pos = m_data.pos = pos; }

	// 反応するキーを設定する
	void SetKey(std::vector<Keyboard::Keys> keys) { m_keys = keys; }

	// 反応するキーを返す
	std::vector<Keyboard::Keys> GetKeys() { return m_keys; }

protected:

	bool m_hitMouseFlag;		// マウスが触れていることを示すフラグ
	bool m_selectFlag;			// 選択状態(ON/OFF)を示すフラグ
	bool m_keySelectFlag;		// キーボード上にて選択されたことを示すフラグ
	bool m_worldInfluenceFlag;	// 触れた際にワールド空間に影響を及ぼすことを示すフラグ

	SimpleMath::Color m_boxColor;

	bool m_activeFlag; //現在触れる状態か

	// 初期情報
	ObjectData2D m_saveData;

	RECT m_rect;

	int m_layer;

	// 選択失敗時震わせる時間遷移用変数
	float m_vibrationTime;

	// ホールド時間を取得する時間遷移用変数
	float m_holdTime;

	// 反応するキー群
	std::vector<Keyboard::Keys> m_keys;

};