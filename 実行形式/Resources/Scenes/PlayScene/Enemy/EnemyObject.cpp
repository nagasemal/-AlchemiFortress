#include "pch.h"
#include "EnemyObject.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/ModelShader.h"

#include "NecromaLib/GameData/Easing.h"

#include "Scenes/PlayScene/Enemy/EnemyManager.h"

// コマンドパターン
#include "Scenes/PlayScene/Enemy/EnemyList/EnemyCommander.h"

// ステートパターン
#include "Scenes/PlayScene/Enemy/State/IEnemyState.h"
#include "Scenes/PlayScene/Enemy/State/EnemyState.h"

// かかる重力
#define GRAVITY 1.2f

// 伸縮：小
#define ELASTICITY_MIN 0.25f
// 伸縮：大
#define ELASTICITY_MAX 0.3f

// モデルの大きさ
#define MODEL_RAGE 4.5f

EnemyObject::EnemyObject(ENEMY_TYPE type, SimpleMath::Vector3 startPos, int lv, float spawnTime) :
	m_power(1),
	m_hp(10),
	m_lv(lv),
	m_accele(),
	m_lengthVec(),
	m_exp(),
	m_stopFlag(),
	m_enemyType(type),
	m_rotation(),
	m_moveVec(),
	m_aliveTimer(spawnTime),
	m_targetPos(),
	m_element(ELEMENT::NOMAL),
	m_gravityScale(),
	m_moveType(),
	m_dethFlag(false)
{

	m_data.pos = startPos;
	m_data.rage = SimpleMath::Vector3(ELASTICITY_MIN);
	
}

EnemyObject::~EnemyObject()
{
}

void EnemyObject::Initialize()
{
	// 待機状態を入れる
	m_stateContext = std::make_shared<EnemyStateContext>(new Enemy_IdlingState());
}

void EnemyObject::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime() * 1.3f;

	// 拡縮アニメーション
	m_data.rage.y = Easing::EaseInCirc(ELASTICITY_MIN, ELASTICITY_MAX, sinf(m_aliveTimer));
	m_data.rage.x = Easing::EaseInCirc(ELASTICITY_MIN, ELASTICITY_MAX, cosf(m_aliveTimer));
	m_data.rage.z = Easing::EaseInCirc(ELASTICITY_MIN, ELASTICITY_MAX, cosf(m_aliveTimer));

	// 距離を取得し正規化する
	m_lengthVec = GetTargetPos() - GetData().pos;
	m_lengthVec.Normalize();

	// 加速度に重力を適応する
	m_gravityScale += GRAVITY * deltaTime;

	// 自身のポインターをコマンドに渡す
	for (auto& command : m_moveCommands)
	{
		command->SetEnemyPtr(*this);
	}

	// 自身のポインターをステートに渡して更新処理を走らせる
	m_stateContext->Update(this);

	// 座標の計算
	m_data.pos += m_lengthVec * m_accele * deltaTime;

	// 重力と跳躍の計算
	m_data.pos.y -= m_gravityScale * deltaTime;
	m_data.pos.y += m_accele.y * deltaTime;

	// Y座標下限値
	if (m_data.pos.y <= 0.0f)
	{
		m_data.pos.y	= 0.0f;
		m_gravityScale	= 0.0f;
	}

	m_stopFlag = false;

	// 初期化
	m_lengthVec = SimpleMath::Vector3();
	m_accele = SimpleMath::Vector3();

	m_modelMatrix =
		SimpleMath::Matrix::CreateFromQuaternion(GetRotate())
		* SimpleMath::Matrix::CreateScale(GetRage() * MODEL_RAGE)
		* SimpleMath::Matrix::CreateTranslation(GetPos());

}

void EnemyObject::Draw()
{
	m_stateContext->RenderIcon(this);
}

void EnemyObject::Render(Model* model)
{
	model->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// 色変更
			lights->SetLightDiffuseColor(0, m_color);
			lights->SetLightDiffuseColor(1, m_color);
			lights->SetLightDiffuseColor(2, m_color);

		});

	m_stateContext->Render(this,model);
}

void EnemyObject::Finalize()
{
	for (auto& command : m_moveCommands)
	{
		delete command;
	}

	m_moveCommands.clear();
}

void EnemyObject::HitMachine(bool flag)
{
	// trueならば処理しない
	if (m_stopFlag == true) return;

	m_stopFlag = flag;

}

void EnemyObject::Bouns()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_data.pos -= (m_lengthVec * (m_accele * 2.0f)) * deltaTime;

}

void EnemyObject::SetEnemyData(Enemy_Data data, EnemyManager* manager)
{
	// lvに応じてパラメータが上昇する
	m_hp		= data.hp		+ (data.hp		* m_lv - 1) / 2;
	m_exp		= data.exp		+ (data.exp		* m_lv - 1) / 2;
	m_power		= data.power	+ (data.power	* m_lv - 1) / 2;
	m_enemyType = data.type;

	if (data.moveType == "ALL") m_moveType = true;
	if (data.moveType == "ONE") m_moveType = false;

	m_element = data.element;
	m_color = ShareJsonData::GetInstance().GetElementColor(m_element);

	// コマンド内容を動かすクラス
	m_commander = std::make_unique<EnemyCommander>();

	for (auto& moveData : data.moveData)
	{
		// 受け取りたい動きの入ったコマンドクラスを取得する
		ICommand_Enemy* command = manager->CreateEnemyMoveCommand(moveData.moveName);
		// 値取得
		MoveParameter moveParam = MoveParameter();
		moveParam.delay = moveData.delay;
		moveParam.time	= moveData.time;
		moveParam.value = moveData.value;
		// コマンドクラスにパラメータを入れる
		command->SetParam(moveParam);
		// 要素分順番に入れる
		m_moveCommands.push_back(command);
	}

	// コマンダークラスにコマンドを登録する
	for (auto& command : m_moveCommands)
	{
		m_commander->AddCommand(command);
	}

}

void EnemyObject::HitBullet(SimpleMath::Vector3 vectol,int power)
{
	// 行動状態に限り処理を行う
	if (m_stateContext->GetState() == IEnemyState::Moveing)

	// 体力を減らす
	m_hp -= (int)power;
	m_stateContext->TransitionTo(new Enemy_KnockBackState(vectol, (float)power));

}
