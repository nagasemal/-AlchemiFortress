#include "pch.h"
#include "DisplayMagicCircle.h"
#include <WICTextureLoader.h>
#include "NecromaLib/GameData/BinaryFile.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"

#define ROTATION_SPEED 0.3f
#define MAX_ALPHA 0.35f


DisplayMagicCircle::DisplayMagicCircle():
	  m_scale(SimpleMath::Vector2::One)
	, m_position(SimpleMath::Vector2::Zero)
	, m_color(1.0f, 1.0f, 1.0f, 1.0f)
	, m_rotationMatrix()
	, m_rotateTime()
	, m_transparentTime()
	, m_easing()
	, m_textureWidth(1280), m_textureHeight(1280)
	, m_windowWidth(), m_windowHeight()
{

	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	m_windowWidth = width;
	m_windowHeight = height;

}

DisplayMagicCircle::~DisplayMagicCircle()
{
}

void DisplayMagicCircle::SetScale(SimpleMath::Vector2 scale)
{
	m_scale = scale;
}
void DisplayMagicCircle::SetPosition(SimpleMath::Vector2 position)
{
	m_position = position;
}

void DisplayMagicCircle::TransparentUpdate(bool spawnFlag)
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	if(spawnFlag)	m_transparentTime += deltaTime * 1.5f;
	else            m_transparentTime -= deltaTime * 1.5f;

	m_easing = Easing::EaseInCirc(0.0f, MAX_ALPHA, m_transparentTime);

}

void DisplayMagicCircle::Update()
{
	float deltaTime = DeltaTime::GetInstance().GetDeltaTime();

	m_rotateTime += deltaTime * ROTATION_SPEED;

	m_rotationMatrix = SimpleMath::Matrix::CreateRotationZ(m_rotateTime);

	m_color.A(m_easing);

}

void DisplayMagicCircle::SpritebatchRender()
{
	auto& pSD = ShareData::GetInstance();
	auto pSB = pSD.GetSpriteBatch();

	SpriteLoder::TextureData texData = SpriteLoder::GetInstance().GetMachineMagicCircleTexture(0);

	pSB->Begin(DirectX::SpriteSortMode_Deferred, pSD.GetCommonStates()->NonPremultiplied());

	// 画像のサイズ
	RECT srcRect = { 0, 0, texData.width, texData.height };

	SimpleMath::Vector2 box_Pos = { static_cast<float>(m_windowWidth) / 2,static_cast<float>(m_windowHeight) / 2 };

	// 選択BOX
	pSB->Draw(texData.tex.Get(), box_Pos, &srcRect,
			  m_color,m_rotateTime, DirectX::XMFLOAT2((float)texData.width / 2, (float)texData.height / 2), 1.5f);

	pSB->End();
}

void DisplayMagicCircle::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}