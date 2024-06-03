#include "pch.h"
#include "TitleLogo.h"

#include <WICTextureLoader.h>
#include "NecromaLib/GameData/BinaryFile.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"
#include "NecromaLib/Singleton/DeltaTime.h"
#include "NecromaLib/GameData/Easing.h"

const std::vector<D3D11_INPUT_ELEMENT_DESC> TitleLogo::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

TitleLogo::TitleLogo():
	  m_windowHeight(0)
	, m_windowWidth(0)
	, m_textureHeight(0)
	, m_textureWidth(0)
	, m_texture(nullptr)
	, m_res(nullptr)
	, m_scale(SimpleMath::Vector2::One)
	, m_position(SimpleMath::Vector2::Zero)
	, m_color(1.0f, 1.0f, 1.0f, 1.0f)
	, m_rotationMatrix()
	, m_animationTime()
	, m_animationSin()
	, m_ripplesData()
{
}

TitleLogo::~TitleLogo()
{
}

void TitleLogo::LoadTexture(const wchar_t* path)
{
	auto device = ShareData::GetInstance().GetDevice();

	DirectX::CreateWICTextureFromFile(device, path, m_res.ReleaseAndGetAddressOf(), m_texture.ReleaseAndGetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_res.As(&tex));

	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;
}

void TitleLogo::Create(const wchar_t* path)
{
	auto device = ShareData::GetInstance().GetDevice();
	auto context = ShareData::GetInstance().GetContext();
	auto resouce = ShareData::GetInstance().GetDeviceResources();
	RECT windowSize = resouce->GetOutputSize();

	m_baseScale = m_scale = { 0.3f,0.3f };

	//シェーダーの作成
	CreateShader();

	//画像の読み込み
	LoadTexture(path);

	SetWindowSize(windowSize.right, windowSize.bottom);

	// プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);

	m_states = std::make_unique<DirectX::CommonStates>(device);

}

void TitleLogo::Update()
{
	m_animationTime += 0.008f;

	m_animationSin = 1.25f + sinf(m_animationTime * 2.5f);

	m_ripplesData += 0.008f;
}

void TitleLogo::Render()
{
	auto context = ShareData::GetInstance().GetContext();
	// 頂点情報
	// Position.xy	:拡縮用スケール
	// Position.z	:アンカータイプ(0～8)の整数で指定
	// Color.xy　	:アンカー座標(ピクセル指定:1280 ×720)
	// Color.zw		:画像サイズ
	// Tex.xy		:x 未使用　y 未使用
	DirectX::VertexPositionColorTexture vertex[1] = {
		DirectX::VertexPositionColorTexture(SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(ANCHOR::MIDDLE_CENTER))
		, SimpleMath::Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))
		, SimpleMath::Vector2(1,1))
	};

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.windowSize = SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);
	cbuff.color = m_color;
	cbuff.diffuse = SimpleMath::Vector4(m_ripplesData, m_animationSin, 1, m_animationTime);

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//　====================[　画像用サンプラーの登録　]
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//　====================[　半透明描画指定　]
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	//　====================[　カリング：左周り　]
	context->RSSetState(m_states->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, SpriteLoder::GetInstance().GetTitleLogo().GetAddressOf());
	context->PSSetShaderResources(1, 1, SpriteLoder::GetInstance().GetMachineMagicCircleTexture(2).tex.GetAddressOf());
	context->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetRule().GetAddressOf());

	//　====================[　インプットレイアウトの登録　]
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void TitleLogo::SetWindowSize(const int& width, const int& height)
{

	m_windowWidth = width;
	m_windowHeight = height;

}

void TitleLogo::SetScale(SimpleMath::Vector2 scale)
{
	m_scale = scale;
}

void TitleLogo::SetPosition(SimpleMath::Vector2 position)
{
	m_position = position;
}

void TitleLogo::CreateShader()
{

	auto device = ShareData::GetInstance().GetDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shader/CutTexture_VS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shader/CutTexture_GS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/CutTexture_PS.cso");

	//インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_cBuffer);

}
