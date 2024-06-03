/*
 *	@File	IEnemyState.h
 *	@Brief	エネミークラスの状態インターフェース
 *	@Date	2023-10-26
 *  @Author Nagase
 */

#pragma once
 // 前方宣言
class EnemyObject;
class EnemyStateContext;

class IEnemyState
{
public:

	// エネミーの現在状況
	enum ENEMY_STATE
	{
		Idoling,
		Moveing,
		Stopping,
		KnockBack,
		Deth,
	};

protected:

	// 現在の状態
	EnemyStateContext* m_enemyContext = nullptr;

	ENEMY_STATE m_state = ENEMY_STATE::Idoling;

public:

	virtual ~IEnemyState() {}

	// 状態をセットする
	void Set_Context(EnemyStateContext* context) { this->m_enemyContext = context; }

	virtual void Update(EnemyObject* object) = 0;
	virtual void Render(EnemyObject* object,Model* model) = 0;
	virtual void RenderIcon(EnemyObject* object) = 0;

	ENEMY_STATE GetState() { return m_state; }

};

class EnemyStateContext
{

private:
	IEnemyState* m_enemyState;

public:
	EnemyStateContext(IEnemyState* state) : 
		m_enemyState(nullptr)
	{
		this->TransitionTo(state);
	}
	~EnemyStateContext() 
	{
		delete m_enemyState; 
	}

	// 次のステートに移行する
	void TransitionTo(IEnemyState* state)
	{
		// 生成されている場合に限り行う
		if (this->m_enemyState != nullptr)

			// 現在のステートを消去
			delete this->m_enemyState;
			
			this->m_enemyState = state;
			this->m_enemyState->Set_Context(this);

		// 新しいステートに入れ替える
		this->m_enemyState = state;
	}

	/// <summary>
	/// 状態に応じて変わる更新処理
	/// </summary>
	void Update(EnemyObject* object) { this->m_enemyState->Update(object); }

	/// <summary>
	/// 状態に応じて変わる描画処理
	/// </summary>
	void Render(EnemyObject* object,Model* model) { this->m_enemyState->Render(object,model); }

	/// <summary>
	/// 状態を可視化するアイコンを描画
	/// </summary>
	/// <param name="object"></param>
	void RenderIcon(EnemyObject* object) { this->m_enemyState->RenderIcon(object); }

	/// <summary>
	/// 現在の状態をENEMY_STATEで返す
	/// </summary>
	IEnemyState::ENEMY_STATE GetState() { return this->m_enemyState->GetState(); }
};
