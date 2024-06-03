#include "pch.h"
#include "DrawSlider.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/InputSupport.h"
#include "NecromaLib/GameData/SpriteCutter.h"
#include "NecromaLib/Singleton/SoundData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/PlayScene/UI/SelectionBox.h"

DrawSlider::DrawSlider(SimpleMath::Vector2 position, SimpleMath::Vector2 scale)
{
    CreateBar(position, scale);
    m_button = std::make_unique<SelectionBox>(position,SimpleMath::Vector2(scale.x,scale.x));

    m_value = 0.5f;

    // 両最端位置の取得
    m_maxPos = SimpleMath::Vector2(position.x + scale.x * (m_gauge_Flont->GetTexture_W() / 2), position.y + scale.y * (m_gauge_Flont->GetTexture_H() / 2));
    m_minPos = SimpleMath::Vector2(position.x - scale.x * (m_gauge_Flont->GetTexture_W() / 2), position.y - scale.y * (m_gauge_Flont->GetTexture_H() / 2));

}

DrawSlider::~DrawSlider()
{
}

void DrawSlider::Initialize()
{

}

void DrawSlider::Update()
{
    m_button->HitMouse();
    InputSupport& pINP = InputSupport::GetInstance();

    SimpleMath::Vector2 mousePos = pINP.GetMousePosScreen();

    bool hitSliderToMouse = PointerToBox(mousePos, m_minPos, m_maxPos);


    if (hitSliderToMouse && pINP.LeftButton_Held())
    {
        m_button->SetHitMouseFlag(true);
        m_button->SetSavePos(SimpleMath::Vector2(mousePos.x, m_button->GetPos().y));

        // マウスの位置から割合を求める
        m_value = (mousePos.x - m_minPos.x) / (m_maxPos.x - m_minPos.x);

    }

}

void DrawSlider::Render(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, RECT rect)
{

    m_gauge_Back->Render();

    m_gauge_Flont->SetRenderRatio(m_value);
    m_gauge_Flont->SetColor(SimpleMath::Color{1.0f,m_value,1.0f - m_value,1.0f });
    m_gauge_Flont->Render();

    m_button->DrawUI(texture, rect,SimpleMath::Color(0.0f,0.0f,0.0f,1.0f));

}

void DrawSlider::Render(int uiIcon)
{

    m_gauge_Back->Render();

    m_gauge_Flont->SetRenderRatio(m_value);
    m_gauge_Flont->SetColor(SimpleMath::Color{ 1.0f,m_value,1.0f - m_value,1.0f });
    m_gauge_Flont->Render();

    m_button->DrawUI(uiIcon);

}

void DrawSlider::Finalize()
{
}

void DrawSlider::CreateBar(SimpleMath::Vector2 position, SimpleMath::Vector2 scale)
{
    auto device = ShareData::GetInstance().GetDeviceResources();

    // ゲージの前後画像を取得
    m_gauge_Flont = std::make_unique<UserInterface>();
    m_gauge_Flont->Create(device
        , L"Resources/Textures/Gauge_main.png"
        , position
        , scale
        , UserInterface::ANCHOR::MIDDLE_CENTER);

    m_gauge_Back = std::make_unique<UserInterface>();
    m_gauge_Back->Create(device
        , L"Resources/Textures/Gauge_base.png"
        , position
        , scale
        , UserInterface::ANCHOR::MIDDLE_CENTER);

    m_button = std::make_unique<SelectionBox>(position, DirectX::SimpleMath::Vector2(scale.y, scale.y));

}
