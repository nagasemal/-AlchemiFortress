#include "pch.h"
#include "DeviceResources.h"
#include "SceneManager.h"

#include "Scenes/TitleScene/TitleScene.h"
#include "Scenes/PlayScene/PlayScene.h"
#include "Scenes/ResultScene/ResultScene.h"
#include "Scenes/SelectScene/SelectScene.h"

#include "NecromaLib/GameData/Particle_2D.h"

#include "NecromaLib/Singleton/SoundData.h"

#include "NecromaLib/GameData/MouseStaging.h"

// 名前空間の利用
using namespace DirectX;

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
SceneManager::SceneManager():
	m_NextScene(GAME_SCENE::TITLE),		// 初期シーンの設定
	m_Scene(nullptr)
{

	Fade::Create();
	m_Fade = Fade::GetInstance();

	DataManager::Create();
	m_DataManager = DataManager::GetInstance();

}

//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
SceneManager::~SceneManager()
{
	Fade::Destroy();

	DataManager::Destroy();
}

//-------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------
void SceneManager::Initialize()
{
	m_Fade->Initialize(1280, 720);
	m_Fade->StartFadeIn();

	// シーン作成
	CreateScene();

	m_NextScene = GAME_SCENE::TITLE;

	m_mouseStaging = std::make_unique<MouseStaging>();
}

//-------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------
void SceneManager::Update(const DX::StepTimer& timer)
{

	InputSupport& pINP = InputSupport::GetInstance();
	m_Fade->Update(timer);
	// ESCキーで終了
	if (pINP.GetKeybordState().IsKeyPressed(Keyboard::Escape)) PostQuitMessage(0);

	// 次のシーンが設定されていたらシーン切り替え
	if (m_NextScene != GAME_SCENE::NONE)
	{
		//  一度だけフェードインのフラグを立ててそれ以降、フェードインならば
		//　シーン遷移は保留とする
		if (!m_Fade->ProcessCheckFadeNow())	m_Fade->StartFadeIn();
		if (m_Fade->ProcessCheckFadeIn())	return;

		// シーン削除
		DeleteScene();
		ShareData::GetInstance().GetCamera()->CameraReset();

		// シーン作成
		CreateScene();

		// BGMを切る
		SoundData::GetInstance().StopBGM();
	}

	//	フェードが終了するまで処理が行われないようにする
	if (m_Fade->ProcessCheckFadeNow()) return;

	// 実態があれば更新
	if (m_Scene != nullptr)
	{
		m_NextScene = m_Scene->Update();
		m_mouseStaging->Update();
	}

	m_DataManager->Update();

	//　デバッグ機能
	{
		// タイトルシーンへ
		if (pINP.GetKeybordState().IsKeyReleased(Keyboard::D1))		m_NextScene = GAME_SCENE::TITLE;

		//　プレイシーンへ
		if (pINP.GetKeybordState().IsKeyReleased(Keyboard::D2))		m_NextScene = GAME_SCENE::PLAY;

		//　リザルトシーン
		if (pINP.GetKeybordState().IsKeyReleased(Keyboard::D3))		m_NextScene = GAME_SCENE::RESULT;

		//　ステージセレクトシーン
		if (pINP.GetKeybordState().IsKeyReleased(Keyboard::D4))		m_NextScene = GAME_SCENE::SELECT;
	}
}

//-------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------
void SceneManager::Render()
{
	// 実態があれば描画
	if (m_Scene != nullptr)
	{
		m_Scene->Draw();
	}

	// 実態があれば描画
	if (m_Scene != nullptr)
	{
		m_Scene->DrawUI();
		m_mouseStaging->Render();
	}

	// フェード用Render
	m_Fade->Render();

}

//-------------------------------------------------------------------
// 後始末
//-------------------------------------------------------------------
void SceneManager::Finalize()
{
	m_Fade->Finalize();
	DeleteScene();
}

/*--------------------------------------------------
シーンの作成
--------------------------------------------------*/
void SceneManager::CreateScene()
{
	// シーンが作成されているときは処理しない
	if (m_Scene != nullptr)				return;

	// 次シーンの作成
	switch (m_NextScene)
	{

	case GAME_SCENE::TITLE:
	{
		m_Scene = new TitleScene();
		break;
	}
	case GAME_SCENE::PLAY:
	{
		m_Scene = new PlayScene();
		break;
	}
	case GAME_SCENE::RESULT:
	{
		m_Scene = new ResultScene();
		break;
	}
	case GAME_SCENE::SELECT:
	{
		m_Scene = new SelectScene();
		break;
	}
	default:
	{
		// 例外なので処理を中断
		return;
	}

	}

	// 作成したシーンを初期化
	m_Scene		->Initialize();

	// 次へのシーン情報を初期化
	m_NextScene = GAME_SCENE::NONE;

	m_NextScene = m_Scene->Update();
}

/*--------------------------------------------------
シーンの削除
--------------------------------------------------*/
void SceneManager::DeleteScene()
{
	// シーンが作成されていなければ処理しない
	if (m_Scene == nullptr)
	{
		return;
	}
	// 現シーンの終了処理
	m_Scene->Finalize();

	// 現シーンの削除
	delete m_Scene;
	m_Scene = nullptr;
}