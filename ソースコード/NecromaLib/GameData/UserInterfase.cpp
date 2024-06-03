#include "pch.h"
#include "UserInterfase.h"
#include <WICTextureLoader.h>

#include "NecromaLib/GameData/BinaryFile.h"

#include "NecromaLib/Singleton/ShareData.h"

#include "NecromaLib/Singleton/SpriteLoder.h"

using namespace DirectX;

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> UserInterface::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// コンストラクタ
UserInterface::UserInterface()
	:m_pDR(nullptr)
	, m_windowHeight(0)
	, m_windowWidth(0)
	, m_textureHeight(0)
	, m_textureWidth(0)
	, m_res(nullptr)
	, m_scale(SimpleMath::Vector2::One)
	, m_position(SimpleMath::Vector2::Zero)
	, m_lightValue(1)
	, m_anchor(ANCHOR::TOP_LEFT)
	, m_renderRatio(1.0f)
	, m_color(1.0f, 1.0f, 1.0f, 1.0f)
	, m_sharpenW(false)
	, m_sharpenH(false)
{
}


/// デストラクタ
UserInterface::~UserInterface()
{
}

/// <summary>
/// テクスチャリソース読み込み関数
/// </summary>
/// <param name="path">相対パス(Resources/Textures/・・・.pngなど）</param>
void UserInterface::LoadTexture(const wchar_t* path)
{

	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, m_res.ReleaseAndGetAddressOf(), m_texture.ReleaseAndGetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_res.As(&tex));

	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;

}

void UserInterface::LoadTexture(std::wstring path)
{

	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path.c_str(), m_res.ReleaseAndGetAddressOf(), m_texture.ReleaseAndGetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_res.As(&tex));

	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;

}

void UserInterface::LoadTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_res.As(&tex));

	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;

}

// 生成関数
void UserInterface::Create(DX::DeviceResources* pDR
	, const wchar_t* path
	, SimpleMath::Vector2 position
	, SimpleMath::Vector2 scale
	, ANCHOR anchor)
{
	m_pDR = pDR;
	auto device = pDR->GetD3DDevice();
	m_position = position;
	m_baseScale = m_scale = scale;
	m_anchor = anchor;

	//シェーダーの作成
	CreateShader();

	//画像の読み込み
	LoadTexture(path);

	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

}

void UserInterface::Create(DX::DeviceResources* pDR, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, SimpleMath::Vector2 position, SimpleMath::Vector2 scale, ANCHOR anchor)
{
	m_pDR = pDR;
	auto device = pDR->GetD3DDevice();
	m_position = position;
	m_baseScale = m_scale = scale;
	m_anchor = anchor;

	//シェーダーの作成
	CreateShader();

	//画像の読み込み
	LoadTexture(texture);

	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);
}

void UserInterface::SetScale(SimpleMath::Vector2 scale)
{
	m_scale = scale;
}
void UserInterface::SetPosition(SimpleMath::Vector2 position)
{
	m_position = position;
}
void UserInterface::SetAnchor(ANCHOR anchor)
{
	m_anchor = anchor;
}

// Shader作成部分だけ分離した関数
void UserInterface::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();

	//　====================[　シェーダーファイルの読み込み　]
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shader/UI_VS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shader/UI_GS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/UI_PS.cso");

	//　====================[　インプットレイアウトの作成　]
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	//　====================[　頂点シェーダーの作成　]
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	//　====================[　ジオメトリシェーダーの作成　]
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	//　====================[　ピクセルシェーダーの作成　]
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//　====================[　シェーダーにデータを渡すためのコンスタントバッファ生成　]
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_cBuffer);
}

// 描画関数
void UserInterface::Render()
{
	//　====================[　デバイスと画面サイズの確保　]
	auto device = ShareData::GetInstance().GetDeviceResources();
	float width = static_cast<float>(device->GetOutputSize().right);
	float height = static_cast<float>(device->GetOutputSize().bottom);

	auto context = m_pDR->GetD3DDeviceContext();
	// 頂点情報
	// Position.xy	:拡縮用スケール
	// Position.z	:アンカータイプ(0～8)の整数で指定
	// Color.xy　	:アンカー座標(ピクセル指定:1280 ×720)
	// Color.zw		:画像サイズ
	// Tex.xy		:削る量,明暗
	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))
		, SimpleMath::Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))
		, SimpleMath::Vector2(m_renderRatio,m_lightValue))
	};

	//　====================[　シェーダーに渡す追加のバッファ　]
	ConstBuffer cbuff;
	cbuff.windowSize = SimpleMath::Vector4(static_cast<float>(width),
										   static_cast<float>(height),
										   static_cast<float>(m_sharpenW),
										   static_cast<float>(m_sharpenH));
	cbuff.color = m_color;

	//　====================[　バッファの更新　]
	//　　|=>　ConstBufferからID3D11Bufferへの変換
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//　====================[　シェーダーにバッファを渡す　]
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//　====================[　画像用サンプラーの登録　]
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);


	//　====================[　半透明描画指定　]
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	//　　|=> 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//　====================[　深度バッファに書き込み　]
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	//　====================[　カリング：左周り　]
	context->RSSetState(m_states->CullNone());

	//　====================[　シェーダをセット　]
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//　====================[　ピクセルシェーダにテクスチャを登録　]
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	if (m_texture_2 != nullptr)
	{
		context->PSSetShaderResources(0, 2, m_texture_2.GetAddressOf());
	}

	//　====================[　インプットレイアウトの登録　]
	context->IASetInputLayout(m_inputLayout.Get());

	//　====================[　板ポリゴンを描画　]
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//　====================[　シェーダの登録を解除　]
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

//void UserInterface::SetWindowSize(const int& width, const int& height)
//{
//	m_windowWidth = width;
//	m_windowHeight = height;
//}
