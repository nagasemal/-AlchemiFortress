#include "pch.h"
#include "Gauge.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/DeltaTime.h"

#include "NecromaLib/GameData/SpriteCutter.h"

#include "Scenes/PlayScene/UI/Number.h"
#include "Scenes/DataManager.h"
#include "SelectionBox.h"

#include <math.h>

#define REDUCE 0.05f
#define TEX_RAGE 64 // 使用するアイコンテクスチャの縦横大きさ

Gauge::Gauge():
    m_difRedioHp(1.0f),
    m_difRedioMp(0.0f),
    m_difRedioCrystal(0.0f),
    m_uiTransparentTime(0.0f)
{
}

Gauge::~Gauge()
{
}

void Gauge::Initialize()
{
    auto device = ShareData::GetInstance().GetDeviceResources();
    device;

    ShareJsonData& pSJD = ShareJsonData::GetInstance();
    UI_Data uiData = pSJD.GetUIData("GaugeOffset");
    float numRender_rate = uiData.option["NUMRENDER_RATE"];

    //　====================[　MPゲージ生成　]
    uiData = pSJD.GetUIData("GaugeMP");
    Add_MP(uiData.pos, uiData.rage, UserInterface::ANCHOR::MIDDLE_CENTER);

    //　　|=>　所持数描画の位置を設定
    SimpleMath::Vector2  numberPos =
        uiData.pos + SimpleMath::Vector2(
            m_base_Mp->GetTexture_W() / numRender_rate * uiData.rage.x,
            m_base_Mp->GetTexture_H() / numRender_rate * uiData.rage.y);

    m_resourceRenderMP = std::make_unique<Number>(numberPos, SimpleMath::Vector2(uiData.option["NUMBER_RAGE"]));
    m_resourceRenderMP->SetColor(SimpleMath::Color(Colors::White));

    //　====================[　Crystalゲージ生成　]
    uiData = pSJD.GetUIData("GaugeCrystal");
    Add_Crystal(uiData.pos, uiData.rage, UserInterface::ANCHOR::MIDDLE_CENTER);

    //　　|=>　所持数描画の位置を設定
    numberPos = uiData.pos + SimpleMath::Vector2(
                m_base_Crystal->GetTexture_W() / numRender_rate * uiData.rage.x,
                m_base_Crystal->GetTexture_H() / numRender_rate * uiData.rage.y);

    m_resourceRenderCrystal = std::make_unique<Number>(numberPos, SimpleMath::Vector2(uiData.option["NUMBER_RAGE"]));
    m_resourceRenderCrystal->SetColor(SimpleMath::Color(Colors::White));

}

void Gauge::Update()
{

    DataManager& pDataM = *DataManager::GetInstance();
    float deltaTime   = DeltaTime::GetInstance().GetNomalDeltaTime();

    //　====================[　MPゲージの処理　]
    //　　|=>　割合計算
    float radio_Mp    = (static_cast<float>(pDataM.GetNowMP()) / static_cast<float>(pDataM.GetNowMP_MAX()));
    m_gauge_Mp        ->SetRenderRatio(radio_Mp);
    //　　|=>　徐々に減るMPの処理
    m_difRedioMp      -= REDUCE * deltaTime;
    m_difRedioMp      = std::min(std::max(m_difRedioMp, radio_Mp), 1.0f);
    m_back_Mp         ->SetRenderRatio(m_difRedioMp);

    //　====================[　クリスタルゲージの処理　]
    //　　|=>　割合計算
    float radio_Crystal = (static_cast<float>(pDataM.GetNowCrystal()) / static_cast<float>(pDataM.GetNowCrystal_MAX()));
    m_gauge_Crystal   ->SetRenderRatio(radio_Crystal);
    //　　|=>　徐々に減るクリスタルの処理
    m_difRedioCrystal -= REDUCE * deltaTime;
    m_difRedioCrystal = std::min(std::max(m_difRedioCrystal, radio_Crystal), 1.0f);
    m_back_Crystal    ->SetRenderRatio(m_difRedioCrystal);
}

void Gauge::Render()
{
    DataManager& pDataM = *DataManager::GetInstance();
    ShareData& pSD      = ShareData::GetInstance();
    SpriteBatch* pSB    = pSD.GetSpriteBatch();
    SpriteLoder& pSL    = SpriteLoder::GetInstance();
    ShareJsonData& pSJD = ShareJsonData::GetInstance();
    UI_Data uiData      = UI_Data();

    //　====================[　魔力リソース描画　]
    m_base_Mp   ->Render();
    m_back_Mp   ->Render();
    m_gauge_Mp  ->Render();

    //　　|=>　リソース数字描画
    m_resourceRenderMP->SetNumber(pDataM.GetNowMP());
    m_resourceRenderMP->Render();

    //　====================[　結晶リソース描画　]
    m_base_Crystal  ->Render();
    m_back_Crystal  ->Render();
    m_gauge_Crystal ->Render();

    //　　|=>　結晶数字描画
    m_resourceRenderCrystal->SetNumber(pDataM.GetNowCrystal());
    m_resourceRenderCrystal->Render();

    //　====================[　リソースアイコン描画開始　]
    pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

    //　====================[　魔力アイコンの描画　]
    uiData = pSJD.GetUIData("GaugeMP");
    RECT rect = SpriteCutter(TEX_RAGE, TEX_RAGE, 0, 0);
    pSB->Draw(pSL.GetElementTexture().Get(), uiData.pos - SimpleMath::Vector2((m_gauge_Mp->GetTexture_W() / 2) * uiData.rage.x, 0.0f),
        &rect, m_resourceRenderMP->GetColor(), 0.0f, SimpleMath::Vector2(TEX_RAGE / 2.0f, TEX_RAGE / 2.0f), uiData.option["NUMBER_RAGE"]);

    //　====================[　結晶アイコンの描画　]
    uiData = pSJD.GetUIData("GaugeCrystal");
    rect = SpriteCutter(TEX_RAGE, TEX_RAGE, 1, 0);
    pSB->Draw(pSL.GetElementTexture().Get(), uiData.pos - SimpleMath::Vector2((m_gauge_Crystal->GetTexture_W() / 2) * uiData.rage.x,0.0f),
        &rect, m_resourceRenderCrystal->GetColor(), 0.0f, SimpleMath::Vector2(TEX_RAGE / 2.0f, TEX_RAGE / 2.0f), uiData.option["NUMBER_RAGE"]);

    pSB->End();

}

void Gauge::Finalize()
{
}

void Gauge::Add_Crystal(SimpleMath::Vector2 position, SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor)
{
    auto device = ShareData::GetInstance().GetDeviceResources();
    auto createGage = [&](UserInterface* object, bool base = false) {

        std::wstring name = L"Resources/Textures/Gauge_main.png";

        if (base) name = L"Resources/Textures/Gauge_base.png";

        object->Create(device
            , name.c_str()
            , position
            , scale
            , anchor);

    };

    m_gauge_Crystal = std::make_unique<UserInterface>();
    createGage(m_gauge_Crystal.get());
    m_gauge_Crystal->SetColor(Colors::MediumPurple);

    m_base_Crystal = std::make_unique<UserInterface>();
    createGage(m_base_Crystal.get(),true);
    m_base_Crystal->SetColor(Colors::IndianRed);

    m_back_Crystal = std::make_unique<UserInterface>();
    createGage(m_back_Crystal.get());
    m_back_Crystal->SetColor(Colors::IndianRed);

}

void Gauge::Add_Hp(SimpleMath::Vector2 position, SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor)
{
    auto device = ShareData::GetInstance().GetDeviceResources();

    auto createGage = [&](UserInterface* object,bool base = false){
        
        std::wstring name = L"Resources/Textures/Gauge_main.png";

        if (base) name = L"Resources/Textures/Gauge_base.png";

        object->Create(device
            , name.c_str()
            , position
            , scale
            , anchor);
    };

    m_gauge_Hp = std::make_unique<UserInterface>();
    createGage(m_gauge_Hp.get());
    m_gauge_Hp->SetColor(Colors::PaleGreen);

    m_base_Hp = std::make_unique<UserInterface>();
    createGage(m_base_Hp.get(),true);
    m_base_Hp->SetColor(Colors::PaleGreen);

    // 背後に描画する赤いバー(徐々に減衰)
    m_back_Hp = std::make_unique<UserInterface>();
    createGage(m_back_Hp.get());
    m_back_Hp->SetColor(Colors::DarkRed);

}

void Gauge::Add_MP(SimpleMath::Vector2 position, SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor)
{
    auto device = ShareData::GetInstance().GetDeviceResources();

    auto createGage = [&](UserInterface* object, bool base = false) {

        std::wstring name = L"Resources/Textures/Gauge_main.png";

        if (base) name = L"Resources/Textures/Gauge_base.png";

        object->Create(device
            , name.c_str()
            , position
            , scale
            , anchor);
    };

    m_gauge_Mp = std::make_unique<UserInterface>();
    createGage(m_gauge_Mp.get());
    m_gauge_Mp->SetColor(Colors::RoyalBlue);

    m_base_Mp = std::make_unique<UserInterface>();
    createGage(m_base_Mp.get(), true);
    m_base_Mp->SetColor(Colors::DarkCyan);

    m_back_Mp = std::make_unique<UserInterface>();
    createGage(m_back_Mp.get());
    m_back_Mp->SetColor(Colors::DarkCyan);

}

SimpleMath::Vector2 Gauge::GetHPGaugePos()
{
    return m_gauge_Hp->GetPosition();
}

SimpleMath::Vector2 Gauge::GetMPGaugePos()
{
    return m_gauge_Mp->GetPosition();
}

SimpleMath::Vector2 Gauge::GetCrystalGaugePos()
{
    return m_gauge_Crystal->GetPosition();
}

void Gauge::TransparentUI(float transparentVal)
{
    m_gauge_Crystal ->SetColor(m_gauge_Crystal  ->GetColorRGB(), transparentVal);
    m_gauge_Mp      ->SetColor(m_gauge_Mp       ->GetColorRGB(), transparentVal);

    m_back_Crystal  ->SetColor(m_back_Crystal   ->GetColorRGB(), transparentVal);
    m_back_Mp       ->SetColor(m_back_Mp        ->GetColorRGB(), transparentVal);

    m_base_Crystal  ->SetColor(m_base_Crystal   ->GetColorRGB(), transparentVal);
    m_base_Mp       ->SetColor(m_base_Mp        ->GetColorRGB(), transparentVal);

    m_resourceRenderCrystal ->SetColor(SimpleMath::Color(m_resourceRenderCrystal->GetColorRGB(), transparentVal));
    m_resourceRenderMP      ->SetColor(SimpleMath::Color(m_resourceRenderMP     ->GetColorRGB(), transparentVal));

}

bool Gauge::GaugeActive()
{
    return m_gauge_Hp->GetColor().A() > 0.0f;
}

float Gauge::GetColorAlpha()
{
    return m_gauge_Hp->GetColor().A();
}
