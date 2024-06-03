#include "pch.h"
#include "BaseLv.h"

#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/ShareJsonData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include "Scenes/PlayScene/UI/Number.h"
#include "Scenes/PlayScene/Field/FieldObjectManager.h"

#define SECOND_COLOR SimpleMath::Color(0.2f,0.2f,0.8f,1.0f);

BaseLv::BaseLv()
{
	auto device = ShareData::GetInstance().GetDevice();

	ShareJsonData& pSJD = ShareJsonData::GetInstance();

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_cBuffer);

	//　====================[　マシンUIの情報を取得　]
	UI_Data uiData = pSJD.GetUIData("GaugeBaseLv");

	auto windowSize = ShareData::GetInstance().GetDeviceResources()->GetOutputSize();
	Create(L"Resources/Textures/MagicCircle/Attacker.png");
	LoadShaderFile(L"BaseLv");

	m_baseLvRender	= std::make_unique<Number>(uiData.pos, uiData.rage * uiData.option["NUMBER_RAGE"]);
	m_baseLvRender	->SetColor(SimpleMath::Color(Colors::Black));

	m_expRedio = 0.0f;
}

BaseLv::~BaseLv()
{
}

void BaseLv::Update(FieldObjectManager* pFieldObjectManager)
{

	auto pPlayerBase = pFieldObjectManager->GetPlayerBase();

	//　====================[　拠点の現在Lvを登録する　]
	m_baseLvRender->SetNumber(pPlayerBase->GetBaseLv());

	//　====================[　拠点経験値の割合計算　]
	m_expRedio = (float)pPlayerBase->GetExp() / (float)pPlayerBase->GetNextLvEXP();

}

void BaseLv::Render(float alphaVal)
{

	auto context = ShareData::GetInstance().GetContext();
	auto windowSize = ShareData::GetInstance().GetDeviceResources()->GetOutputSize();

	ShareJsonData& pSJD = ShareJsonData::GetInstance();
	UI_Data uiData = pSJD.GetUIData("GaugeBaseLv");

	// 頂点情報
	// Position.xy	:拡縮用スケール
	// Position.z	:アンカータイプ(0～8)の整数で指定
	// Color.xy　	:アンカー座標(ピクセル指定:1280 ×720)
	// Color.zw		:画像サイズ
	// Tex.xy		:ウィンドウサイズ（バッファも同じ。こちらは未使用）
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(SimpleMath::Vector3(uiData.rage.x, uiData.rage.y, static_cast<float>(MIDDLE_CENTER))
		, SimpleMath::Vector4(uiData.pos.x,uiData.pos.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))
		, SimpleMath::Vector2(m_expRedio,0.8f))
	};

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.windowSize	= SimpleMath::Vector4(static_cast<float>(windowSize.right), static_cast<float>(windowSize.bottom), 1, 1);
	cbuff.base_color	= SimpleMath::Color(1.0f, 1.0f, 1.0f, alphaVal);
	cbuff.second_color	= SimpleMath::Color(0.0f, 1.0f, 1.0f, alphaVal);

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context			->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	context			->VSSetConstantBuffers(0, 1, cb);
	context			->GSSetConstantBuffers(0, 1, cb);
	context			->PSSetConstantBuffers(0, 1, cb);

	AdvanceRender();

	//ピクセルシェーダにテクスチャを登録する。
	context			->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context			->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetMagicRule(0).GetAddressOf());

	// 板ポリゴンを描画
	m_batch			->Begin();
	m_batch			->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch			->End();

	//シェーダの登録を解除しておく
	context			->VSSetShader(nullptr, nullptr, 0);
	context			->GSSetShader(nullptr, nullptr, 0);
	context			->PSSetShader(nullptr, nullptr, 0);

	// 拠点Lvの描画
	m_baseLvRender	->SetPosition(uiData.pos);
	m_baseLvRender	->SetRage(uiData.rage * uiData.option["NUMBER_RAGE"]);
	m_baseLvRender	->SetColor(SimpleMath::Color(m_baseLvRender->GetColorRGB(), alphaVal));
	m_baseLvRender	->Render();

}
