#pragma once

#include <GeometricPrimitive.h> // 立方体、球、ティーポットなど
#include <Effects.h>        // 頂点の彩色、ライティング、テクスチャなど

#include "StepTimer.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/Singleton/ShareData.h"

#include "Fade.h"
#include "SceneObj.h"
#include "Scenes/DataManager.h"

class MouseStaging;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

    void Initialize();
    void Update(const DX::StepTimer& timer);
    void Render();
    void Finalize();

    // シーン関係処理
    // シーンの作成
    void CreateScene();

    // シーンの削除
    void DeleteScene();

private:

    // 次のシーン番号
    GAME_SCENE m_NextScene;

    // シーン
    SceneObj* m_Scene;

    Fade* m_Fade;

    // 共有する変数の管理
    DataManager* m_DataManager;

    // スプライトバッチ
    std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

    // スプライトフォント
    std::unique_ptr<DirectX::SpriteFont> m_SpriteFont;


    // マウスの演出処理
    std::unique_ptr<MouseStaging> m_mouseStaging;

};