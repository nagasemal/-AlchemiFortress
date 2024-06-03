#pragma once
#include "NecromaLib/GameData/GameObject3D.h"

#include "Scenes/PlayScene/Mouse/MousePointer.h"
#include "Scenes/PlayScene/Enemy/EnemyObject.h"
#include "Scenes/PlayScene/AlchemicalMachine/Bullet/Bullet.h"
#include "Scenes/PlayScene/UI/SelectionBox.h"
#include "Scenes/PlayScene/Field/FieldObjectManager.h"
#include <list>
#include <vector>

#include "Scenes/DataManager.h"

#include "NecromaLib/GameData/CommonStruct.h"

class EnemyObject;
class Bullet;
class SelectionBox;

class AlchemicalMachineObject : public GameObjct3D
{
public:

	AlchemicalMachineObject();
	virtual ~AlchemicalMachineObject() {};

	// 選択時の特別なアップデート処理
	virtual void SelectUpdate() = 0;

	// 選択時、マウス選択可能なUIの表示に使用
	virtual void RenderUI() = 0;

	// 共通してアップデートをかける処理
	void Update_Common();

	// 共通してアップデートをかける処理
	void SelectUpdate_Common();

	// 共通して描画するUIの処理
	void SelectRenderUI_Common();

	// HP残量をビルボード描画する
	void RenderHP();

	// クリック誘導を描画する
	void RenderClickPrompt();

	// 他のオブジェクト同士の当たり判定
	void HitToMouse(MousePointer* pMP);

	// 効果範囲内に3Dオブジェクトが入った
	bool OnCollisionEnter_MagicCircle(GameObjct3D* object);
	
	/// <summary>
	/// モデル描画系
	/// </summary>
	/// <param name="model">　　　 ベースのモデル　</param>
	/// <param name="ring">　　　　追加パーツ</param>
	void ModelRender(DirectX::Model* model,
					 DirectX::Model* ring = nullptr,
					 bool silhouette = false);

	// 召喚
	void SummonAM(SimpleMath::Vector3 pos);

// アクセサ
public:

	// 存在しているか否か
	const bool GetActive()									const { return m_active; }

	// マウスが触れているか否か
	const bool GetHitMouse()								const { return m_hitMouseFlag;}

	// 死んで消えるアニメーションが終わったことを示す
	const bool GetDethFlag()								const { return m_hp <= 0.0f && m_dethTime >= 1.0f;}

	// オブジェクトの名前
	const std::string GetObjectName()						const { return m_objectName; }

	// オブジェクトのタイプ
	const MACHINE_TYPE GetModelID()							const { return m_machineID;}

	// 効力を返してくれます
	const float GetMachineEffectValue()						const { return m_machineEffectValue; }

	// 効果発動までの時間
	const float GetSpan()									const { return m_span;}
	
	// 現在レベル
	const int	  GetLv()									const { return m_lv; }

	// 現在HP
	const int	  GetHP()									const { return m_hp; }

	// 現在MAXHP
	const int	  GetMAXHP()								const { return m_maxHp; }

	// 効果範囲
	const Circle GetMagicCircle()							const { return m_magicCircle; }

	// 保有している色情報
	// HPが0ならば黒を返します
	const SimpleMath::Color GetColor()						const { return m_hp <= 0 ? SimpleMath::Color(0, 0, 0, 1): m_color * (1.0f - m_invincibleTime);}

	// 何かの要因で変化を受けている状態
	const bool GetPowerUpFlag()								const { return m_powerUPFlag; }

	// 現在の属性
	const ELEMENT GetElement()								const { return m_element; }

	// 現在どのライン上に存在しているか
	const int GetLine()										const { return m_line; }

	// 次のLVに必要なクリスタルの量
	const int GetNextLvCrystal();

	// 修繕に必要なクリスタルの量
	const int GetRepairCrystal();

	// 解体で得られるクリスタルの量
	const int GetDismantlingCrystal();

	// 選択されている状態か否かを決定付ける
	const int GetSelectModeFlag() const { return m_selectModeFlag; }

	const float GetPopTextTime() const { return m_popHPTime; }

	// 現在クリック誘導状態であるか
	const bool GetClickPromptFlag() const { return m_clickPromptFlag; }

	void SetLv(int lv)									{ m_lv = lv; }
	void SetHP(int hp)									{ m_hp = hp; }
	void SetMAXHP(int maxHP)							{ m_maxHp = maxHP;}
	void SetPos(SimpleMath::Vector3 pos)				{ m_data.pos = pos; }
	void SetMagicCircle(Circle circle)					{ m_magicCircle = circle;}
	void SetActive(bool flag)							{ m_active = flag; }
	void SetLine(int line)								{ m_line = line; }
	void SetPowerUpFlag(bool flag)						{ m_powerUPFlag = flag; }
	void SetSelectModeFlag(bool flag)					{ m_selectModeFlag = flag;}
	void SetHitMouseFlag(bool flag)						{ m_hitMouseFlag = flag; }
	void SetElement(ELEMENT element)					{ m_element = element; }
	void SetColor(SimpleMath::Color color)				{ m_color = color;}
	void SetClickPromptFlag(bool flag)					{ m_clickPromptFlag = flag; }

public:
	// シルエット描画
	void SilhouetteRender(DirectX::Model* model, SimpleMath::Matrix matrix);

	// 通常描画
	void NomalRender(DirectX::Model* model, SimpleMath::Matrix matrix,SimpleMath::Color color = SimpleMath::Color());

	// 半透明描画
	void TransparentRender(DirectX::Model* model, SimpleMath::Matrix matrix);

	// シェーダー無し描画
	void NotShaderRender(DirectX::Model* model, SimpleMath::Matrix matrix);

	// デプス書き込み描画
	void WriteDepathRender(DirectX::Model* model,DirectX::Model* secondModel);

private:

	// 出現時演出に使用する時間変数
	float m_spawnTime;

	// 死亡時演出に使用する時間変数
	float m_dethTime;

protected:

	// Lvの最大値
	static const int MAX_LV = 5;

	// マシンID　キーとしてモデルの受け取りを行う
	MACHINE_TYPE m_machineID;

	// マシンの耐久値
	int m_hp;

	// マシンの最大耐久値
	int m_maxHp;

	// マシンのレベル
	int m_lv;

	// マシン特有のデータ(攻撃力や防御性能、採掘速度など)を決定付けるデータ
	float m_machineEffectValue;

	// 次の効果発動までの時間
	float m_span;

	// 無敵時間,無敵状態
	float m_invincibleTime;
	bool m_invincibleFlag;

	// Y軸回転させるための変数
	float m_rotateAnimation;

	// HPが徐々に減るようにする変数
	float m_difRedioHp;

	// オブジェクトのライン位置(どの円形線状にあるか)
	int m_line;

	// 存在しているか否か
	bool m_active;

	// 何かしらの影響で強化を受けているか
	bool m_powerUPFlag;

	// 選択モード時のフラグ
	bool m_selectModeFlag;

	// マウスが当たっているか否か
	bool m_hitMouseFlag;

	// 選択されている間の時間
	float m_selectModeTime;

	// クリックするように促す
	bool m_clickPromptFlag;

	// 効果範囲
	Circle m_magicCircle;

	std::string m_objectName;

	// オブジェクトの色
	SimpleMath::Color m_color;

	// サブオブジェクトの色
	SimpleMath::Color m_subColor;

	// オブジェクトの属性
	ELEMENT m_element;

	// UIの位置を決める
	SimpleMath::Vector3 m_offsetUIPos;

	// 触れたら出現するHPバーUI用の時間変数
	float m_popHPTime;
};