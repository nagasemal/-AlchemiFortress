#include "pch.h"
#include "DorpShadow.h"
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include "NecromaLib/GameData/BinaryFile.h"
#include "NecromaLib/Singleton/ShareData.h"

#define RAD_90 1.5708f

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> DorpShadow::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

DorpShadow::DorpShadow():
	m_proj(),
	m_world(),
	m_view()
{
}

DorpShadow::~DorpShadow()
{
}

void DorpShadow::Initialize()
{
	auto device = ShareData::GetInstance().GetDevice();

	//シェーダーの作成
	CreateShader();

	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Shadow.png", nullptr, m_texture.ReleaseAndGetAddressOf());

	// プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(ShareData::GetInstance().GetContext());

	m_states = std::make_unique <DirectX::CommonStates > (device);
}

void DorpShadow::Update()
{
}

void DorpShadow::CreateShadow(SimpleMath::Vector3 pos)
{
	// 丸影の位置を調整する
	pos.y = -1.0f;

	m_vertices.push_back({pos,SimpleMath::Color(0.2f,0.2f,0.2f,0.2f),SimpleMath::Vector2(2.0f, 0.0f)});
}

void DorpShadow::DeleteShadow()
{
	// 全て消去
	m_vertices.clear();

}

void DorpShadow::CreateWorld()
{

	m_world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::CreateRotationX(RAD_90);

	m_world = rot * m_world;
}


void DorpShadow::Render()
{
	auto context = ShareData::GetInstance().GetContext();
	auto view = ShareData::GetInstance().GetView();
	auto proj = ShareData::GetInstance().GetProjection();

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.m_matView = view.Transpose();
	cbuff.m_matProj = proj.Transpose();
	cbuff.m_matWorld = m_world.Transpose();
	cbuff.m_diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//　====================[　画像用サンプラーの登録　]
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap()};
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
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//　====================[　インプットレイアウトの登録　]
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	m_batch->Begin();

	// 要素分回す
	for (auto& vertexs : m_vertices)
	{
		m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertexs, 1);
	}
	
	m_batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void DorpShadow::Finalize()
{
	m_texture.Reset();
}

void DorpShadow::CreateShader()
{
	auto device = ShareData::GetInstance().GetDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shader/Shadow_VS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shader/Shadow_GS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/Shadow_PS.cso");

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
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}
