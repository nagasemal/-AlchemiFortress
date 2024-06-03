#include "pch.h"
#include "EnemyManager.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineManager.h"
#include "Scenes/DataManager.h"
#include <algorithm>
#include <random>

#include "DirectXHelpers.h"

#include "NecromaLib/Singleton/ShareJsonData.h"

#include "Scenes/PlayScene/Enemy/EnemyList/Enemy_StandardMove.h"
#include "Scenes/PlayScene/Enemy/EnemyList/Enemy_MeanderingMove.h"
#include "Scenes/PlayScene/Enemy/EnemyList/Enemy_HoppingMove.h"

// 拠点からスポーン地点までの最低距離
#define SPAWN_MIN 20
// 拠点からスポーン地点までの最大距離
#define SPAWN_MAX 40

EnemyManager::EnemyManager() :
	m_timer(),
	m_totalTimer(),
	m_nextEnemyTime(),
	m_knokDownEnemyType(ENEMY_TYPE::ENMEY_NONE),
	m_knokDownFlag(0),
	m_enemyNums(),
	m_falmeTotalEnemyExp()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize()
{

	ShareData& pSD = ShareData::GetInstance();

	m_testBox = GeometricPrimitive::CreateBox(pSD.GetContext(), SimpleMath::Vector3(1,1,1));
	m_enemyObject = std::make_unique<std::list<EnemyObject>>();

	//m_particle_hit = std::make_unique<Particle>(Particle::HIT_BULLET);
	//m_particle_hit->Initialize();

	m_particle_spawn = std::make_unique<Particle>(Particle::SPAWN_ENEMY);
	m_particle_spawn->Initialize();

	m_particle_delete = std::make_unique<Particle>(Particle::DELETE_ENEMY);
	m_particle_delete->Initialize();

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(pSD.GetDevice());
	fx->SetDirectory(L"Resources/Models");

	m_enemyModel = DirectX::Model::CreateFromCMO(pSD.GetDevice(), L"Resources/Models/Slime.cmo", *fx);

	// エネミーの数値取得
	auto pSJD = &ShareJsonData::GetInstance(); 
	pSJD->LoadingJsonFile_Enemy();

	ReloadEnemyData();

	// エフェクトを作成
	m_effect = std::make_unique<BasicEffect>(ShareData::GetInstance().GetDevice());
	m_effect->SetTextureEnabled(true);
	m_effect->SetVertexColorEnabled(true);
	m_effect->SetLightingEnabled(false);

	// 入力レイアウトを作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect(
			ShareData::GetInstance().GetDevice(),
			m_effect.get(),
			VertexPositionColorTexture::InputElements,
			VertexPositionColorTexture::InputElementCount,
			m_inputLayout.ReleaseAndGetAddressOf())
	);
}

void EnemyManager::Update(SimpleMath::Vector3 basePos)
{
	// 常にリセットをかける
	m_falmeTotalEnemyExp = 0;
	basePos;

	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	DataManager& pDM = *DataManager::GetInstance();

	// 毎秒初期化
	m_knokDownFlag = 0;
	m_knokDownEnemyType = ENEMY_TYPE::ENMEY_NONE;

	m_timer += deltaTime;
	m_totalTimer += deltaTime;

	// 更新処理
	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		// 生成された瞬間を取得してパーティクルを出す
		m_particle_spawn->OnShot(it->GetPos(), it->GetAliveTimer() <= 0.0f);

		// 子クラスからfalseで消す
		if (it->GetDethFlag())
		{
			pDM.SetNowEnemyKill(pDM.GetNowEnemyKill() + 1);

			m_knokDownFlag++;
			m_knokDownEnemyType = it->GetEnemyType();

			// EXPを獲得出来るようにする
			m_falmeTotalEnemyExp += it->GetEXP();

			m_particle_delete->OnShot(it->GetPos(), true);

			it->Finalize();
			it = m_enemyObject->erase(it);

			if (it == m_enemyObject->end()) break;
		}

		it->Update();
	}

	m_particle_spawn->UpdateParticle();
	m_particle_delete->UpdateParticle();
}

void EnemyManager::Render()
{

	m_particle_delete	->Render();
	m_particle_spawn	->Render();

	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		it->Render(m_enemyModel.get());
	}

}

void EnemyManager::RenderUI()
{
	ShareData& pSD = ShareData::GetInstance();
	auto status = pSD.GetCommonStates();

	// ビルボードされたUIを描画するための設定
	auto camera = ShareData::GetInstance().GetCamera();
	auto context = pSD.GetContext();
	// スクリーン座標はY軸が＋－逆なので
	SimpleMath::Matrix invertY = SimpleMath::Matrix::CreateScale(1.0f, -1.0f, 1.0f);

	// ビュー行列の回転を打ち消す行列を作成する
	SimpleMath::Matrix invView = camera->GetViewMatrix().Invert();
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	// エフェクトにビュー行列と射影行列を設定する
	m_effect->SetView(camera->GetViewMatrix());
	m_effect->SetProjection(camera->GetProjectionMatrix());

	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{

		pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, status->NonPremultiplied(), nullptr, status->DepthNone(), status->CullCounterClockwise(), [=]
			{
				// ワールド行列作成
				SimpleMath::Matrix world =
					SimpleMath::Matrix::CreateScale(0.01f) *
					invertY *
					invView *
					SimpleMath::Matrix::CreateTranslation(it->GetPos() + SimpleMath::Vector3(0.0f, 1.0f, 0.0f));

				// エフェクトを適応する
				m_effect->SetWorld(world);
				m_effect->Apply(context);
				// 入力レイアウトを設定する
				context->IASetInputLayout(m_inputLayout.Get());
			});

		it->Draw();


		pSD.GetSpriteBatch()->End();
	}

}

void EnemyManager::Finalize()
{
	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		it->Finalize();
		//delete& it;
	}
	m_enemyObject->clear();
	m_enemyObject.reset();

	delete m_enemyObject.get();

	m_testBox.reset();
}

void EnemyManager::ReloadEnemyData()
{
	m_enemyNums = 0;
	m_timer = 0;

	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	// 中身が存在しない場合は飛ばす
	if(pSJD.GetStageData().enemys_Spawn.size() == 0) return;

	// ランダム生成でない場合初めから生成をしておく
	if (pSJD.GetStageData().enemys_Spawn[0].type != ENEMY_TYPE::ENMEY_NONE)
	{
		// 初めから出現させるエネミーの情報を取得しておく
		for (int i = 0; i < pSJD.GetStageData().enemys_Spawn.size(); i++)
		{

			EnemyObject object = CreateEnemy(ENEMY_TYPE::SLIME, i);
			m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));
		
		}
	}
	else
	{
		// 初めから出現させるエネミーの情報を取得しておく
		for (int i = 0; i < pSJD.GetStageData().condition[MISSION_TYPE::ENEMY_KILL][0].value; i++)
		{

			EnemyObject object = GetRandomEnemy();
			object.SetAliveTimer(pSJD.GetStageData().enemys_Spawn[0].spawnTime * (1 + i));
			m_enemyObject->push_back(*std::make_unique<EnemyObject>(object));

		}
	}

}

//　Jsonファイルから読み取った情報を元にエネミーを製造する
EnemyObject EnemyManager::CreateEnemy(ENEMY_TYPE type,int spawnNumber)
{
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	Enemys_Spawn enemySpawn = pSJD.GetStageData().enemys_Spawn[spawnNumber];

	// エネミーオブジェクトを生成
	EnemyObject enemy(type, enemySpawn.spawnPos, enemySpawn.lv,enemySpawn.spawnTime);

	// エネミーのパラメータを生成
	Enemy_Data enemyData = pSJD.GetEnemyData(pSJD.GetStageData().enemys_Spawn[spawnNumber].type);

	// 初期化処理
	enemy.Initialize();

	// エネミーのパラメータを入れる
	enemy.SetEnemyData(enemyData,this);

	return enemy;
}

EnemyObject EnemyManager::GetRandomEnemy()
{
	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	std::uniform_int_distribution<> dist_enemyType(1,ENEMY_NUM - 1);
	std::random_device rd;
	int enemyType_rand = static_cast<int>(dist_enemyType(rd));

	std::random_device seed;
	std::default_random_engine engine(seed());
	// 円周上のどこに設置するかを決める
	std::uniform_real_distribution<> dist(0, XM_2PI);
	// 拠点からの距離を取得
	std::uniform_int_distribution<> dist2(SPAWN_MIN, SPAWN_MAX);
	std::mt19937 gen(rd());
	float rand = static_cast<float>(dist(engine));
	float rand2 = static_cast<float>(dist2(gen));

	EnemyObject enemy((ENEMY_TYPE)enemyType_rand, SimpleMath::Vector3(rand2 * cosf(rand), 1.0f, rand2 * sinf(rand)), 1, 0.0f);
	Enemy_Data enemyData = pSJD.GetEnemyData((ENEMY_TYPE)enemyType_rand);

	enemy.Initialize();

	enemy.SetEnemyData(enemyData,this);

	return enemy;
}

// アルケミカルマシンtoエネミー  呼び出し元含めて二重for文で回しているため、数がかさむと重くなる。
void EnemyManager::HitAMObejct(AlchemicalMachineObject* alchemicalMachines)
{
	//　現存存在するエネミー分回す
	for (std::list<EnemyObject>::iterator it = m_enemyObject->begin(); it != m_enemyObject->end(); it++)
	{
		// 反発
		it->SetStopFlag(CircleCollider(it->GetCircle(), alchemicalMachines->GetCircle()));
	}

}

int EnemyManager::GetNockDownEnemyExp()
{
	return m_falmeTotalEnemyExp;
}

ICommand_Enemy* EnemyManager::CreateEnemyMoveCommand(const std::string moveName)
{
	if (moveName == "Standard")		return new Enemy_StanderdMove();
	if (moveName == "Meandering")	return new Enemy_MeanderingMove();
	if (moveName == "Hopping")		return new Enemy_HoppingMove();

	return nullptr;
}