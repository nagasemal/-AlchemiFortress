#include "pch.h"
#include "MachineGauge.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/PlayScene/AlchemicalMachine/AlchemicalMachineObject.h"

MachineGauge::MachineGauge()
{
}

MachineGauge::~MachineGauge()
{
}

void MachineGauge::Update(int hp, int maxHp)
{
    hp;
    maxHp;

}

void MachineGauge::Render(AlchemicalMachineObject* gameObject)
{

    ShareData& pSD = ShareData::GetInstance();
    SpriteLoder::TextureData baseTexData = SpriteLoder::GetInstance().GetBaseGage();
    SpriteLoder::TextureData mainTexData = SpriteLoder::GetInstance().GetMainGage();

    auto pSB = pSD.GetSpriteBatch();

    RECT rect = { 0,0,baseTexData.width,baseTexData.height };

    pSB->Draw(baseTexData.tex.Get(),
        gameObject->GetPos(),
        &rect,
        Colors::White,
        0.0f,
        SimpleMath::Vector2(static_cast<float>(baseTexData.width / 2),
                            static_cast<float>(baseTexData.height / 2)),
        0.4f);

}
