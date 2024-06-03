#include "pch.h"
#include "EnemyState.h"

#include "NecromaLib/Singleton/ModelShader.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"

#include "Scenes/PlayScene/Enemy/EnemyList/ICommand_Enemy.h"
#include "Scenes/PlayScene/Enemy/EnemyList/EnemyCommander.h"
#include "Scenes/PlayScene/Enemy/EnemyObject.h"

#include "Scenes/PlayScene/Shadow/MagicCircle.h"

#include "NecromaLib/GameData/SpriteCutter.h"

#define ENEMY_MAGICCIRCLE_NUM 6

#define TEX_W 64
#define TEX_H 64

#define TEX_RAGE 2.0f

// 出現予告時間
#define SPAWN_TIME 5.0f

// 出現予告魔法陣サイズ
#define MAGICCIRCLE_SIZE 1.0f

Enemy_IdlingState::Enemy_IdlingState()
{

	m_state = ENEMY_STATE::Idoling;

	m_magicCircle = std::make_unique<MagicCircle>();
	m_magicCircle->Initialize();

}

void Enemy_IdlingState::Update(EnemyObject* object)
{
	object->SetAliveTimer(object->GetAliveTimer() - DeltaTime::GetInstance().GetDeltaTime());

	// 当たり判定をとらない
	object->SetColliderActive(false);

	m_magicCircle->Update();

	// 5秒前に魔法陣を展開する
	if (object->GetAliveTimer() <= SPAWN_TIME)
	{
		m_magicCircle->CreateMagicCircle(object->GetData().pos, MAGICCIRCLE_SIZE,SimpleMath::Color(Colors::Black));
	}

	//  生存時間が0.0fになったら召喚する
	if (object->GetAliveTimer() <= 0.0f)
	{

		// 魔法陣を削除する
		m_magicCircle->DeleteMagicCircle();
		m_enemyContext->TransitionTo(new Enemy_MoveingState());

	}
}

void Enemy_IdlingState::Render(EnemyObject* object,Model* model)
{
	object;
	model;

	m_magicCircle->CreateWorld();
	m_magicCircle->Render(ENEMY_MAGICCIRCLE_NUM);

}

void Enemy_IdlingState::RenderIcon(EnemyObject* object)
{
	auto pSD = &ShareData::GetInstance();

	// 出現時間を描画する
	if (object->GetAliveTimer() <= SPAWN_TIME)
	{

		RECT rect = RECT{ TEX_W * (int)object->GetAliveTimer() + 1 ,0,TEX_W + TEX_H * (int)object->GetAliveTimer() + 1,TEX_H};

		pSD->GetSpriteBatch()->Draw(SpriteLoder::GetInstance().GetNumberTexture().Get(),
			object->GetPos(),
			&rect, Colors::Black, 0.0f, SimpleMath::Vector2(TEX_W / 2, TEX_H / 2), TEX_RAGE);

	}
}



Enemy_MoveingState::Enemy_MoveingState()
{

	m_state = ENEMY_STATE::Moveing;

}

void Enemy_MoveingState::Update(EnemyObject* object)
{

	object->SetAliveTimer(object->GetAliveTimer() + DeltaTime::GetInstance().GetDeltaTime());
	// 当たり判定をとる
	object->SetColliderActive(true);

	//　移動コマンド再生種類の切り替え
	if (object->GetMoveType()) object->GetEnemyCommand()->Execute_ALL();
	else					   object->GetEnemyCommand()->Execute_One();

	//　動かさないようにする
	if (object->GetStopFlag()) object->Bouns();

	// ターゲット方向に身体を向ける
	SimpleMath::Vector3 length = object->GetLengthVec();
	length.y = 0.0f;
	object->SetLengthVec(length);
	object->SetRotate(SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, length));

	// 死亡状態に遷移する
	if (object->GetHp() <= 0.0f)
	{
		m_enemyContext->TransitionTo(new Enemy_DethState());
	}

}

void Enemy_MoveingState::Render(EnemyObject* object,Model* model)
{
	ShareData& pSD = ShareData::GetInstance();

	// 重なった際、影を描画
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().SilhouetteShader();
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			//　====================[　深度ステンシルステートの設定　]
			pSD.GetContext()->OMSetDepthStencilState(nullptr, 3);
		});
}

void Enemy_MoveingState::RenderIcon(EnemyObject* object)
{
	auto pSD = &ShareData::GetInstance();

	RECT rect = SpriteCutter(TEX_W, TEX_H,0,0);

	pSD->GetSpriteBatch()->Draw(SpriteLoder::GetInstance().GetEnemyStateTexture().Get(),
		object->GetPos(),
		&rect, Colors::White, 0.0f, SimpleMath::Vector2(TEX_W / 2, TEX_H / 2), TEX_RAGE);

}



Enemy_StoppingState::Enemy_StoppingState()
{

	m_state = ENEMY_STATE::Stopping;

}

void Enemy_StoppingState::Update(EnemyObject* object)
{

	// 塔やマシンから離れたら再び動き出す
	if (!object->GetStopFlag())
	{
		m_enemyContext->TransitionTo(new Enemy_MoveingState());
	}

}

void Enemy_StoppingState::Render(EnemyObject* object,Model* model)
{
	ShareData& pSD = ShareData::GetInstance();

	// 重なった際、影を描画
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().SilhouetteShader();
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			//　====================[　深度ステンシルステートの設定　]
			pSD.GetContext()->OMSetDepthStencilState(nullptr, 3);
		});
}

void Enemy_StoppingState::RenderIcon(EnemyObject* object)
{
	object;
}



Enemy_KnockBackState::Enemy_KnockBackState(SimpleMath::Vector3 direction, float power)
{

	m_state = ENEMY_STATE::Idoling;

	m_knockBackVec = direction;
	m_knockBackVec.y = power / 5.0f;

	m_knockBackVec.Normalize();

}

void Enemy_KnockBackState::Update(EnemyObject* object)
{

	m_knockBackVec.y -= DeltaTime::GetInstance().GetDeltaTime();

	// 当たり判定をとらない
	object->SetColliderActive(false);

	object->AddAccele(m_knockBackVec);

	// ターゲット方向に身体を向ける
	SimpleMath::Vector3 length = object->GetLengthVec();
	length.y = 0.0f;
	object->SetLengthVec(length);
	object->SetRotate(SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::UnitX, length));

	// 地面に付いたら再び動き出す
	if (m_knockBackVec.y <= 0.0f && object->GetPos().y <= 0.0f)
	{
		m_enemyContext->TransitionTo(new Enemy_MoveingState());
	}
}

void Enemy_KnockBackState::Render(EnemyObject* object,Model* model)
{

	ShareData& pSD = ShareData::GetInstance();

	SimpleMath::Color color = SimpleMath::Color(0.75f + m_knockBackVec.y, 0.75f, 0.75f, 1.0f);

	model->UpdateEffects([&](IEffect* effect)
		{
			// 今回はライトだけ欲しい
			auto lights = dynamic_cast<IEffectLights*>(effect);
			// 色変更
			lights->SetLightDiffuseColor(0, color);
			lights->SetLightDiffuseColor(1, color);
			lights->SetLightDiffuseColor(2, color);

		});


	// 重なった際、影を描画
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			ModelShader::GetInstance().SilhouetteShader();
		});

	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{
			//　====================[　深度ステンシルステートの設定　]
			pSD.GetContext()->OMSetDepthStencilState(nullptr, 3);
		});

}

void Enemy_KnockBackState::RenderIcon(EnemyObject* object)
{
	auto pSD = &ShareData::GetInstance();

	RECT rect = SpriteCutter(TEX_W, TEX_H, 1, 0);

	pSD->GetSpriteBatch()->Draw(SpriteLoder::GetInstance().GetEnemyStateTexture().Get(),
		object->GetPos(),
		&rect, Colors::White, 0.0f, SimpleMath::Vector2(TEX_W / 2, TEX_H / 2), TEX_RAGE);
}

Enemy_DethState::Enemy_DethState()
{
	m_dethTime = 0.0f;
	m_state = ENEMY_STATE::Deth;

}

void Enemy_DethState::Update(EnemyObject* object)
{

	// 当たり判定をとらない
	object->SetColliderActive(false);

	m_dethTime += DeltaTime::GetInstance().GetDeltaTime() * 2.5f;

	object->SetAccele(SimpleMath::Vector3(0.0f,m_dethTime,0.0f));
	object->GravityReset();

	// 一定時間経ったら消す
	object->SetDethFlag(m_dethTime >= DETH_TIME);

}

void Enemy_DethState::Render(EnemyObject* object,Model* model)
{
	ShareData& pSD = ShareData::GetInstance();

	// 半透明描画
	model->Draw(pSD.GetContext(), *pSD.GetCommonStates(), object->GetModleMatrix(), pSD.GetView(), pSD.GetProjection(), false, [&]
		{

			//　====================[　深度ステンシルステートの設定　]
			ModelShader::GetInstance().ToransparentShader();

		});
}

void Enemy_DethState::RenderIcon(EnemyObject* object)
{
	auto pSD = &ShareData::GetInstance();

	RECT rect = SpriteCutter(TEX_W, TEX_H, 2, 0);

	pSD->GetSpriteBatch()->Draw(SpriteLoder::GetInstance().GetEnemyStateTexture().Get(),
		object->GetPos(),
		&rect, Colors::White, 0.0f, SimpleMath::Vector2(TEX_W / 2, TEX_H / 2), TEX_RAGE);

}
