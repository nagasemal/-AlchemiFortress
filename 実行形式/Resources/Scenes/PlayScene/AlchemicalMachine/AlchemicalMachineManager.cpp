#include "pch.h"
#include "AlchemicalMachineManager.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/Singleton/SoundData.h"

#include "Scenes/Commons/DrawVariableNum.h"

#include "DirectXHelpers.h"

#include "Scenes/PlayScene/Camera/MoveCamera.h"
#include "AM_None.h"
#include "AM_Attacker.h"
#include "AM_Defenser.h"
#include "AM_Mining.h"
#include "AM_Recovery.h"
#include "AM_Upper.h"

// 魔力を回復する間隔
#define MPPLUSTIME 2.0f
// 魔力の回復量
#define MPPLUSNUM  1.0f
// 魔力を回収しはじめる最低マシン量
#define MPPUSLMACHINE 8

// 円周の一番小さい場合におけるマシンの最大数
#define CIRCLE_MAX_MIN  4

// 円周の最大ライン
#define CIRCLE_MAX_LINE 9

// ライン同士の間隔
#define CIRCLE_LINE_DISTANCE 5.5f

// 魔法陣出現の高さ(大)
#define MAGICCIRCLE_HEIGHT	0.3f 

// バレットの大きさ
#define BULLET_RAGE 0.75f

// ワールド空間に出すUIの大きさ調整
#define WORLD_UI_SIZE 0.01f

// ワールド空間に出すUIの位置調整
#define WORLD_UI_POSY 4.5f
// クリック誘導時ワールド空間に出すUIの位置調整
#define CLICKPROMPT_POS SimpleMath::Vector3(1.0f,-1.0f,0.0f)

AlchemicalMachineManager::AlchemicalMachineManager() :
	m_allHitObjectToMouse(),
	m_selectNumber(-1),
	m_prevSelectMachinePos(0, 0, 0),
	m_mpPulsTimer(),
	m_AMnums{ 0,0,0,0,0,0 },
	m_saveWheelValue(0),
	m_scrollValue(),
	m_rotationStop(),
	m_crystalPulsVal(),
	m_mpPulsVal(),
	m_spawnMachine		(MACHINE_TYPE::NONE),
	m_lvUpMachine		(MACHINE_TYPE::NONE)
{
}

AlchemicalMachineManager::~AlchemicalMachineManager()
{
}

void AlchemicalMachineManager::Initialize()
{
	ShareData& pSD = ShareData::GetInstance();

	//　====================[　クラス取得　]

	//　　|=>　マシンUIの描画クラス
	m_selectManager		= std::make_unique<MachineSelectManager>();
	m_selectManager		->TextuerLoader();
	m_selectManager		->Initialize();

	//　　|=>　マシンのモデルパッククラス
	m_AMFilter = std::make_unique<AlchemicalMachineFilter>();

	//　　|=>　バレットに使用するモデル
	m_testBox = GeometricPrimitive::CreateSphere(pSD.GetContext(),0.75f);

	//　　|=>　丸影描画クラス
	m_dorpShadow		= std::make_unique<DorpShadow>();
	m_dorpShadow		->Initialize();

	//　　|=>　[パーティクル] バレットToエネミー
	m_particle_hit		= std::make_unique<Particle>(Particle::HIT_BULLET);
	m_particle_hit		->Initialize();

	//　　|=>　[パーティクル] マシン設置時
	m_particle_Put		= std::make_unique<Particle>(Particle::MACHINE_SPAWN);
	m_particle_Put		->Initialize();

	//　　|=>　[パーティクル] ディフェンサー攻撃時
	m_particle_Gurd		= std::make_unique<Particle>(Particle::DEFENSE_EFFECT);
	m_particle_Gurd		->Initialize();

	//　　|=>　[パーティクル] クリスタル回収時
	m_particle_Mining	= std::make_unique<Particle>(Particle::MINING_EFFECT);
	m_particle_Mining	->Initialize(L"Crystal");
	m_particle_Mining	->SetParticleSpawnTime(1.0f);

	//　　|=>　[パーティクル] 魔力回収時
	m_particle_Recovery = std::make_unique<Particle>(Particle::RECOVERY_EFFECT);
	m_particle_Recovery	->Initialize(L"MP");
	m_particle_Recovery	->SetParticleSpawnTime(1.0f);

	//　　|=>　[パーティクル] バレットの軌跡
	m_particle_Bullet	= std::make_unique<Particle>(Particle::BULLET_LINE);
	m_particle_Bullet	->Initialize();

	//　　|=>　マシン用魔法陣描画クラス
	m_magicCircle		= std::make_unique<MagicCircle>();
	m_magicCircle		->Initialize();

	//　　|=>　拠点用魔法陣描画クラス
	m_magicCircle_Field = std::make_unique<MagicCircle>();
	m_magicCircle_Field	->Initialize();


	//　====================[　マシンの生成　]
	CreateAMMachine();
	
	//　====================[　現在場に出せるマシンの設定　]
	LvToObjectActives(1);
	
	//　====================[　Jsonからリソースを取得する　]
	JsonLoadResources();

	//　====================[　ビルボード描画に使用　]
	//　　|=> エフェクトを作成
	m_effect = std::make_unique<BasicEffect>(ShareData::GetInstance().GetDevice());
	m_effect->SetTextureEnabled(true);
	m_effect->SetVertexColorEnabled(true);
	m_effect->SetLightingEnabled(false);

	//　　|=> 入力レイアウトを作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect(
			ShareData::GetInstance().GetDevice(),
			m_effect.get(),
			VertexPositionColorTexture::InputElements,
			VertexPositionColorTexture::InputElementCount,
			m_inputLayout.ReleaseAndGetAddressOf())
	);

}

void AlchemicalMachineManager::Update(
	FieldObjectManager* fieldManager,
	MousePointer* pMP,
	EnemyManager* enemys,
	MoveCamera* moveCamera)
{
	InputSupport& pINP = InputSupport::GetInstance();
	DataManager& pDM = *DataManager::GetInstance();
	PlayerBase* pPlayerBase = fieldManager->GetPlayerBase();

	//　====================[　入力関係　]
	auto mouse			= pINP.GetMouseState();
	auto keyboard		= pINP.GetKeybordState();
	bool leftRelease	= pINP.LeftButton_Release();

	//　====================[　回転の停止/再開　]
	if (pINP.RightButton_Press())
	{
		m_rotationStop = !m_rotationStop;
	}

	//　====================[　設置個所の設定　]
	LvToObjectActives(pPlayerBase->GetBaseLv());

	//　====================[　マシンの数をカウントする変数　]
	//　　|=>　全てのマシン
	int amNum			= 0;
	//　　|=>　Noneマシン
	int amNum_Nomal		= 0;
	
	//　====================[　Noneマシンのある配列番号を得る(LVの関係で無いものは-1)　]
	int noneFirst		= -1;

	//　====================[　リソースの取得量のリセット　]
	m_mpPulsVal			= 0;
	m_crystalPulsVal	= 0;

	//　====================[　マシンToマウスの判定リセット　]
	m_allHitObjectToMouse = false;

	//　====================[　通知系変数のリセット　]
	m_spawnMachine = m_lvUpMachine = MACHINE_TYPE::NONE;

	//　====================[　魔力回復周期　]
	m_mpPulsTimer += DeltaTime::GetInstance().GetDeltaTime();

	//　====================[　パーティクルの更新　]
	Update_Particle();

	//　====================[　選択状態の解除　]
	if (leftRelease)
	{
		// カメラ移動用の時間変数を可能ならばリセットする
		moveCamera->ResetTargetChangeTimer();

		// 選択されたオブジェクトがない場合の処理
		if (m_selectNumber != -1)
		{
			m_prevSelectMachinePos = m_AMObject[m_selectNumber]->GetPos();
		}
		else
		{
			m_prevSelectMachinePos = SimpleMath::Vector3();
		}

		// 選択状態の解除
		if (!pINP.GetHitUI())
		{
			m_selectNumber = -1;
		}
	}

	//　====================[　セレクトマネージャーの更新処理　]
	m_selectManager->Update(fieldManager);

	//　====================[　選択中のオブジェクトがある場合の処理　]
	if (m_selectNumber != -1)
	{
		//　注視点移動
		moveCamera->TargetChange(m_AMObject[m_selectNumber]->GetData().pos);

		//　選択済みのオブジェクトの選択時アップデートを回す
		m_AMObject[m_selectNumber]->SelectUpdate();
		m_AMObject[m_selectNumber]->SelectUpdate_Common();

		//　選択オブジェクトに魔法陣展開
		m_magicCircle->CreateMagicCircle(
			m_AMObject[m_selectNumber]->GetPos(),
			m_AMObject[m_selectNumber]->GetMagicCircle().r,
			m_AMObject[m_selectNumber]->GetColor());

	}
	//　====================[　選択中のオブジェクトがない場合の処理　]
	else
	{
		m_magicCircle		->DeleteMagicCircle();

		//　注視点移動
		moveCamera->TargetChange({ 0,0,0 });
	}

	//　====================[　登録した影情報を消す　]
	m_dorpShadow->DeleteShadow();

	//　====================[　全マシンの更新処理　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		//　================[　マシンを動かす　]
		MovingMachine(i);
		m_AMObject[i]->Update_Common();

		//　================[　マシンToマウス　]
		if (m_AMObject[i]->GetHitMouse() && m_AMObject[i]->GetActive())
		{

			m_allHitObjectToMouse = true;

			// クリックで選択状態に移行
			if (leftRelease && m_selectNumber != i)
			{
				// 選択されているマシンのインデックス番号を渡す
				m_selectNumber = i;
			}
		}

		m_AMObject[i]->SetSelectModeFlag(m_selectNumber == i);

		//　存在していれば処理を続ける
		if (!m_AMObject[i]->GetActive()) continue;

		// アルケミカルマシンの更新処理
		m_AMObject[i]->Update();
		m_AMObject[i]->HitToMouse(pMP);

		if (m_AMObject[i]->GetModelID() == MACHINE_TYPE::NONE)
		{
			// Noneの始めの配列番号を得る
			if (noneFirst <= -1)
			{
				noneFirst = i;
			}

			// 現状場に存在しているマシンの総数を調べる (None)
			amNum_Nomal++;

			// 所持数0ならば表示を行わない
			if (m_AMnums[m_selectManager->GetSelectMachineType()] <= 0)
			{
				m_AMObject[i]->SetActive(false);
			}
			else
			{
				m_AMObject[i]->SetActive(true);
			}

		}
		
		// 全体数
		amNum++;

		if (m_AMObject[i]->GetActive())
		{
			m_dorpShadow->CreateShadow(m_AMObject[i]->GetData().pos);
		}

		// MP追加処理 (インデックスがリカバリーを指し、魔力回復周期が来た時、回転を止めていれば魔力を回復させる)
		if (m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop && m_AMObject[i]->GetModelID() == MACHINE_TYPE::RECOVERY)
		{
			// リカバリーマシンが生きていたら加算を行う
			m_mpPulsVal += (int)m_AMObject[i]->GetHP() > 0 ? (int)m_AMObject[i]->GetMachineEffectValue() * m_AMObject[i]->GetLv() : 0;
		}

		// マシンの耐久値が0になった
		if (m_AMObject[i]->GetDethFlag())
		{
			Dismantling(i);
		}

	}

	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// アルケミカルマシンの個別更新処理
		Update_Upper(i, enemys);
		Update_Attacker(i, enemys);
		Update_Defenser(i, enemys);
		Update_Mining(i, fieldManager, enemys);
		Update_Recovery(i, enemys);
	}

	//　====================[　マシンを召喚する処理　]
	SpawnAMMachine((leftRelease && !pINP.GetHitUI() && m_allHitObjectToMouse));

	//　====================[　魔力リソースの増加　]
	//　　|=>　周期が来る　回転が止まっていない
	if (m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop)
	{
		m_mpPulsTimer	= 0;
		m_mpPulsVal		+= ((int)MPPLUSNUM * ((amNum - amNum_Nomal) / MPPUSLMACHINE));
		pDM.SetNowMP(pDM.GetNowMP() + m_mpPulsVal);
	}

	//　====================[　クリスタルリソースの増加　]
	pDM.SetNowCrystal(pDM.GetNowCrystal() + m_crystalPulsVal);


	//　====================[　所持数の増加　]
	if (m_selectManager->GetManufacturingFlag())
	{
		m_AMnums[m_selectManager->GetSelectMachineType()]++;

		// 自動設置モードならば即座に配置を行う
		if (m_selectManager->GetAutomaticFlag() && noneFirst >= 0)
		{

			m_selectNumber = noneFirst;
			//　====================[　マシンを召喚する処理　]
			SpawnAMMachine(true);
		}

	}

	//　====================[　マウスの当たり判定を戻す　]
	if(leftRelease)  pMP->ReleaseLeftButtom();

	//　====================[　バレット更新処理　]
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Update();
		m_particle_Bullet->Update(it->get()->GetPos(),true,it->get()->GetColor());

		// 子クラスからfalseで消す
		if ((it)->get()->deleteRequest())
		{
			m_particle_hit->OnShot(it->get()->GetPos(),true,it->get()->GetColor());
			it = m_bullets.erase(it);
			if (it == m_bullets.end()) break;
		}
	}

	SimpleMath::Color circleColor = SimpleMath::Color(1.0f, !m_rotationStop, !m_rotationStop,1.0f);

	//　====================[　魔法陣の位置を定める　]
	m_magicCircle_Field->CreateMagicCircle(
		SimpleMath::Vector3{ 0,MAGICCIRCLE_HEIGHT,0 },
		pPlayerBase->GetBaseLv() * CIRCLE_LINE_DISTANCE, circleColor);

	if (!m_rotationStop)
	{
		m_magicCircle_Field->CreateWorld();
	}

}

void AlchemicalMachineManager::Render()
{

	//　====================[　置いた際に出すパーティクル　]
	//　　|=>　描画順の影響で先に描画
	m_particle_Put->Render();

	//　====================[　シルエット描画用ドローコール　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// 存在しているかチェック
		if (m_AMObject[i]->GetActive())
		{

			// モデルの描画			オブジェクトに割り当てられたIDをもとにモデル配列からデータを取り出す
			m_AMObject[i]->ModelRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()),
									   m_AMFilter->GetRingModel(m_AMObject[i]->GetModelID()),true);
		}
	}

	//　====================[　通常描画用ドローコール　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// 存在しているかチェック
		if (m_AMObject[i]->GetActive())
		{
			// モデルの描画			オブジェクトに割り当てられたIDをもとにモデル配列からデータを取り出す
			m_AMObject[i]->ModelRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()),
									   m_AMFilter->GetRingModel(m_AMObject[i]->GetModelID()), false);
		
			m_AMObject[i]->Draw();
		
		}
	}

	//　====================[　マシンが選択されている　]
	if (m_selectNumber != -1)
	{
		//　　|=>　マシンの魔法陣描画処理
		m_magicCircle->CreateWorld();
		m_magicCircle->Render(m_AMObject[m_selectNumber]->GetModelID());
	}

	
	//　====================[　丸影の描画処理　]
	m_dorpShadow->CreateWorld();
	m_dorpShadow->Render();

	//　====================[　パーティクルの描画処理　]
	m_particle_hit		->Render();
	m_particle_Gurd		->Render();
	m_particle_Mining	->Render();
	m_particle_Recovery	->Render();
	m_particle_Bullet	->Render();

	//　====================[　バレットの描画処理　]
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Render(m_testBox.get());
	}

	//　====================[　拠点の魔法陣描画処理　]
	m_magicCircle_Field->Render(0);

}

void AlchemicalMachineManager::WriteDepath()
{
	//　====================[　影描画用ドローコール　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// 存在しているかチェック
		if (m_AMObject[i]->GetActive())
		{
			m_AMObject[i]->WriteDepathRender(m_AMFilter->HandOverAMModel(m_AMObject[i]->GetModelID()),
											 m_AMFilter->GetRingModel(m_AMObject[i]->GetModelID()));
		}
	}
}


void AlchemicalMachineManager::DrawUI()
{
	//　====================[　マシンUIの指定可視化UIの描画　]
	m_selectManager->Render();

	//　====================[　UIのビルボード描画　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		// 存在しなければそれ以降も存在しないため、処理を終了する
		if (!m_AMObject[i]->GetActive()) break;
		// 選択番号と一致している場合は二回ドローコールをする必要はない為、飛ばす
		if (m_selectNumber == i) continue;

		// マウスの周辺にあるマシンの情報開示 
		Circle mouseCircle = Circle(InputSupport::GetInstance().GetMousePosWolrd(),5.0f);

		// 表示条件 : マウスカーソルの範囲内　出現実現時間が0より上(UIが表示されている)
		// 表示内容 : マシンの耐久値 
		if (PointerToCircle(mouseCircle, m_AMObject[i]->GetPos()) ||
			m_AMObject[i]->GetPopTextTime() > 0.0f)
		{

			//========  開始　========//
			BillboardRenderUI_Start(i, SimpleMath::Vector3(0.0f, WORLD_UI_POSY, 0.0f));

			m_AMObject[i]->RenderHP();

			//========  終了　========//
			BillboardRenderUI_End();

		}

		// 表示条件 : マシンが選択誘導状態になっている
		if (m_AMObject[i]->GetClickPromptFlag())
		{
			//========  開始　========//
			BillboardRenderUI_Start(i, CLICKPROMPT_POS);

			m_AMObject[i]->RenderClickPrompt();

			//========  終了　========//
			BillboardRenderUI_End();
		}
	}

	//　====================[　マシンUIに関連する描画　]
	//　　|=>　錬金ボタンUI
	//　　|=>　マシンの名前
	//　　|=>　必要リソース量
	m_selectManager->RenderUI(m_AMnums);

	//　====================[　UIの表示　]
	if (m_selectNumber != -1)
	{
		//========  開始　========//
		BillboardRenderUI_Start(m_selectNumber, SimpleMath::Vector3(0.0f, WORLD_UI_POSY, 0.0f));

		m_AMObject[m_selectNumber]->RenderHP();
		//========  終了　========//
		BillboardRenderUI_End();

	}
}

void AlchemicalMachineManager::Finalize()
{

	//　====================[　全マシンの終了処理　]
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		m_AMObject[i]->Finalize();
		m_AMObject[i].reset();
	}

	m_AMObject.clear();
	m_AMObject.shrink_to_fit();

	//　====================[　全バレットの終了処理　]
	for (std::list<std::unique_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		it->get()->Finalize();
	}

	m_bullets.clear();

	//　====================[　UI描画クラスの終了処理　]
	m_selectManager->Finalize();
	m_selectManager.reset();

	m_AMFilter.reset();
	m_testBox.reset();

}

void AlchemicalMachineManager::BillboardRenderUI_End()
{

	ShareData& pSD = ShareData::GetInstance();
	pSD.GetSpriteBatch()->End();
}

void AlchemicalMachineManager::BillboardRenderUI_Start(int index, SimpleMath::Vector3 position)
{
	ShareData& pSD = ShareData::GetInstance();
	auto status = pSD.GetCommonStates();
	auto camera = ShareData::GetInstance().GetCamera();
	auto context = pSD.GetContext();

	//　====================[　ビルボード行列生成　]
	//　　|=>　スクリーン座標はY軸が＋－逆なので-1をかける
	SimpleMath::Matrix invertY = SimpleMath::Matrix::CreateScale(1.0f, -1.0f, 1.0f);

	//　　|=> ビュー行列の回転を打ち消す行列を作成する
	SimpleMath::Matrix invView = camera->GetViewMatrix().Invert();
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	//　　|=> エフェクトにビュー行列と射影行列を設定する
	m_effect->SetView(camera->GetViewMatrix());
	m_effect->SetProjection(camera->GetProjectionMatrix());


	//　====================[　UIをワールド空間に出す]
	pSD.GetSpriteBatch()->Begin(SpriteSortMode_Deferred, status->NonPremultiplied(), nullptr, status->DepthNone(), status->CullCounterClockwise(), [=]
	{	
		// ワールド行列作成
		SimpleMath::Matrix world =
			SimpleMath::Matrix::CreateScale(WORLD_UI_SIZE) *
			invertY *
			invView *
			SimpleMath::Matrix::CreateTranslation(m_AMObject[index]->GetPos() + position);

		// エフェクトを適応する
		m_effect->SetWorld(world);
		m_effect->Apply(context);

		// 入力レイアウトを設定する
		context->IASetInputLayout(m_inputLayout.Get());
	});

}

Model* AlchemicalMachineManager::GetSelectModel()
{
	return m_AMFilter->HandOverAMModel(m_selectManager->GetSelectMachineType());
}

void AlchemicalMachineManager::ReloadResource()
{

	auto pSJD = &ShareJsonData::GetInstance();
	Stage_Resource resource = pSJD->GetStageData().resource;

	//　====================[　マシン所持数を追加　]
	m_AMnums[MACHINE_TYPE::ATTACKER]	+= resource.attacker;
	m_AMnums[MACHINE_TYPE::DEFENSER]	+= resource.deffencer;
	m_AMnums[MACHINE_TYPE::UPPER]		+= resource.upper;
	m_AMnums[MACHINE_TYPE::RECOVERY]	+= resource.recovery;
	m_AMnums[MACHINE_TYPE::MINING]		+= resource.mining;

}

void AlchemicalMachineManager::Update_Particle()
{

	//　====================[　パーティクル群の更新処理　]
	m_particle_hit			->UpdateParticle();
	m_particle_Put			->UpdateParticle();
	m_particle_Gurd			->UpdateParticle();
	m_particle_Mining		->UpdateParticle();
	m_particle_Recovery		->UpdateParticle();
	m_particle_Bullet		->UpdateParticle();
}

void AlchemicalMachineManager::Update_Attacker(int index, EnemyManager* enemys)
{
	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::ATTACKER) return;


	AM_Attacker* attacker = dynamic_cast<AM_Attacker*>(m_AMObject[index].get());

	//　====================[　バレットの発射リクエスト　]
	if (attacker->BulletRequest(enemys->GetEnemyData()))
	{
		m_bullets.push_back(std::make_unique<Bullet>(attacker->GetBulletData()));
	}
}

void AlchemicalMachineManager::Update_Defenser(int index, EnemyManager* enemys)
{
	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::DEFENSER) return;

	AM_Defenser* defenser = dynamic_cast<AM_Defenser*>(m_AMObject[index].get());

	//　====================[　エネミーとの衝突時　]
	defenser->EnemyHit(enemys->GetEnemyData());

	//　　|=>　専用のパーティクルを出す
	m_particle_Gurd->Update(defenser->GetPos(), defenser->CounterAttack(), defenser->GetColor());

}

void AlchemicalMachineManager::Update_Mining(int index, FieldObjectManager* fieldManager, EnemyManager* enemys)
{
	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::MINING) return;

	AM_Mining* mining = dynamic_cast<AM_Mining*>(m_AMObject[index].get());

	//　====================[　エネミーとの衝突時　]
	mining->HitEnemy(enemys->GetEnemyData());

	//　====================[　クリスタルリソースの回収　]
	m_crystalPulsVal += mining->AllFieldObject(fieldManager);

	//　　|=>　専用のパーティクルを出す
	m_particle_Mining->Update(mining->GetPos(), !mining->GetCrystalFlag(),mining->GetColor());

}

void AlchemicalMachineManager::Update_Recovery(int index,EnemyManager* enemys)
{

	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::RECOVERY) return;

	AM_Recovery* recovery = dynamic_cast<AM_Recovery*>(m_AMObject[index].get());

	//　====================[　エネミーとの衝突時　]
	recovery->HitEnemy(enemys->GetEnemyData());

	//　====================[　回収状態ならばパーティクルを出す　]
	m_particle_Recovery->OnShot(recovery->GetPos(), m_mpPulsTimer >= MPPLUSTIME && !m_rotationStop, recovery->GetColor());
}

void AlchemicalMachineManager::Update_Upper(int index,EnemyManager* enemyManager)
{

	if (m_AMObject[index]->GetHP() <= 0) return;
	if (m_AMObject[index]->GetModelID() != MACHINE_TYPE::UPPER) return;

	AM_Upper* upper = dynamic_cast<AM_Upper*>(m_AMObject[index].get());

	//　====================[　エネミーとの衝突時　]
	upper->HitEnemy(enemyManager->GetEnemyData());

	//　====================[　マシンの配列を渡す　]
	upper->MachinePowerUP(m_AMObject);

}

void AlchemicalMachineManager::MovingMachine(int number)
{
	//　====================[　早期リターン　]
	//　　|=>　回転が停止している
	if (m_rotationStop) return;

	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	//　====================[　回転速度取得　]
	int rotateSpeed = (int)ShareJsonData::GetInstance().GetGameParameter().rotateSpeed;

	//　====================[　原点を中心に回転移動　]
	SimpleMath::Matrix matrix = SimpleMath::Matrix::Identity;
	matrix *= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(rotateSpeed * deltaTime));
	//　　|=>  回転後の座標をマシンに代入
	m_AMObject[number]->SetPos(SimpleMath::Vector3::Transform(m_AMObject[number]->GetPos(), matrix));

}

// Lvに応じて置ける場所が増える為、設定をする
void AlchemicalMachineManager::CreateAMMachine()
{
	int counter = 0;

	//　====================[　影の配列をリセットする　]
	m_dorpShadow->DeleteShadow();

	int circle_max_line = ShareJsonData::GetInstance().GetGameParameter().baseLV_MAX;

	//　====================[　配置箇所を決める　]
	for (int i = 1; i < circle_max_line; i++)
	{
		for (int j = 0; j < CIRCLE_MAX_MIN * i; j++)
		{

			// Noneマシンを追加
			m_AMObject.push_back(m_AMFilter->HandOverAMClass(MACHINE_TYPE::NONE));

			// 初期化処理
			m_AMObject[counter]->Initialize();

			// 存在するライン決め
			m_AMObject[counter]->SetLine(i);
			m_AMObject[counter]->SummonAM(SetVelocityCircle(j, CIRCLE_MAX_MIN * i, i * CIRCLE_LINE_DISTANCE));
			m_AMObject[counter]->SetActive(false);

			m_dorpShadow->CreateShadow(m_AMObject[counter]->GetData().pos);

			counter++;

		}
	}

}

void AlchemicalMachineManager::LvToObjectActives(int lineNumber)
{
	for (int i = 0; i < m_AMObject.size(); i++)
	{
		if (m_AMObject[i]->GetModelID() != MACHINE_TYPE::NONE) continue;

		if (m_AMObject[i]->GetLine() <= lineNumber)
		{
			m_AMObject[i]->SetActive(true);
		}
		else
		{
			m_AMObject[i]->SetActive(false);
		}
	}
}

void AlchemicalMachineManager::SpawnAMMachine(bool leftButtom)
{

	//　====================[　マシンの召喚処理　]
	//　　|=>　 選択ボックスUIに触れていない
	//　　|=>　 対象オブジェクトに触れている
	//　　|=>　 説明UIに触れていない
	//　　|=>　 左ボタンを離すとオブジェクトを入れ替える
	if (leftButtom)
	{
		// MachineType::Noneを選択している場合に限り処理を通す
		if (m_AMObject[m_selectNumber]->GetModelID() != MACHINE_TYPE::NONE) return;

		// 召喚できるオブジェクトの数が足りない場合は召喚しない
		if (m_AMnums[m_selectManager->GetSelectMachineType()] <= 0) return;

		// 位置情報を取得
		SimpleMath::Vector3 savePos = m_AMObject[m_selectNumber]->GetData().pos;

		// ライン情報を取得
		int saveLine = m_AMObject[m_selectNumber]->GetLine();

		// 本取得
		m_AMObject[m_selectNumber] = m_AMFilter->HandOverAMClass(m_selectManager->GetSelectMachineType());

		// 初期化
		m_AMObject[m_selectNumber]->Initialize();

		// Noneと対象のオブジェクトを入れ替える
		m_AMObject[m_selectNumber]->SummonAM(savePos);

		// ライン情報を引き継ぐ
		m_AMObject[m_selectNumber]->SetLine(saveLine);

		// 召喚したオブジェクトの保有数を一つ減らす
		m_AMnums[m_selectManager->GetSelectMachineType()]--;
		
		// 召喚したマシンのIDを取得
		m_spawnMachine = m_AMObject[m_selectNumber]->GetModelID();

		// 召喚用のパーティクルを出現させる
		m_particle_Put->OnShot(m_AMObject[m_selectNumber]->GetPos(),true, m_AMObject[m_selectNumber]->GetColor());
		m_particle_Put->OnShot(m_AMObject[m_selectNumber]->GetPos(), true, SimpleMath::Color(Colors::Cyan));

		// 召喚用の音声を鳴らす
		SoundData::GetInstance().PlaySE(XACT_WAVEBANK_SES_WATERMACHINEATTACK);
	}
}

void AlchemicalMachineManager::Dismantling(int index)
{

	// 位置情報を取得
	SimpleMath::Vector3 savePos = m_AMObject[index]->GetData().pos;

	// ライン情報を取得
	int saveLine		= m_AMObject[index]->GetLine();

	// 死亡用のパーティクルを出現させる
	m_particle_Put		->OnShot(savePos, true,	SimpleMath::Color(Colors::Black));
	m_particle_Put		->OnShot(savePos, true, SimpleMath::Color(Colors::IndianRed));

	// 本取得
	m_AMObject[index]	= m_AMFilter->HandOverAMClass(MACHINE_TYPE::NONE);

	// 初期化
	m_AMObject[index]	->Initialize();

	// Noneと対象のオブジェクトを入れ替える
	m_AMObject[index]	->SummonAM(savePos);

	// ライン情報を引き継ぐ
	m_AMObject[index]	->SetLine(saveLine);

	// 破壊用の音声を鳴らす
	SoundData::GetInstance().PlaySE(XACT_WAVEBANK_SES_FIREMACHINEATTACK);

}

void AlchemicalMachineManager::JsonLoadResources()
{
	auto pSJD = &ShareJsonData::GetInstance();

	Stage_Resource resource = pSJD->GetStageData().resource;

	m_AMnums[MACHINE_TYPE::ATTACKER]	= resource.attacker;
	m_AMnums[MACHINE_TYPE::DEFENSER]	= resource.deffencer;
	m_AMnums[MACHINE_TYPE::UPPER]		= resource.upper;
	m_AMnums[MACHINE_TYPE::RECOVERY]	= resource.recovery;
	m_AMnums[MACHINE_TYPE::MINING]		= resource.mining;

	for (int i = 0; i < pSJD->GetStageData().machine.size(); i++)
	{
		// 位置情報を取得
		SimpleMath::Vector3 savePos = m_AMObject[i]->GetData().pos;

		// ライン情報を取得
		int saveLine = m_AMObject[i]->GetLine();

		// 本取得
		m_AMObject[i] = m_AMFilter->HandOverAMClass(pSJD->GetStageData().machine[i].type);

		// 初期化
		m_AMObject[i]->Initialize();

		// 属性設定
		m_AMObject[i]->SetElement(pSJD->GetStageData().machine[i].element);
		m_AMObject[i]->SetColor(ChangeData::ChangeColor(pSJD->GetStageData().machine[i].element));

		// 属性設定
		m_AMObject[i]->SetLv(pSJD->GetStageData().machine[i].lv);

		// Noneと対象のオブジェクトを入れ替える
		m_AMObject[i]->SummonAM(savePos);

		// ライン情報を引き継ぐ
		m_AMObject[i]->SetLine(saveLine);

	}
}
