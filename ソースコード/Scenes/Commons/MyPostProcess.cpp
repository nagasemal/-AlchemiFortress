#include "pch.h"
#include "MyPostProcess.h"

#include "NecromaLib/GameData/BinaryFile.h"
#include "NecromaLib/Singleton/ShareData.h"
#include "NecromaLib/Singleton/SpriteLoder.h"

#include <stdio.h>
#include <wchar.h>

const std::vector<D3D11_INPUT_ELEMENT_DESC> MyPostProcess::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

MyPostProcess::MyPostProcess():
	m_blerVal(),
	m_chromaticVal()
{
}

MyPostProcess::~MyPostProcess()
{
}

void MyPostProcess::CreateShader()
{
	auto device = ShareData::GetInstance().GetDevice();
	auto context = ShareData::GetInstance().GetContext();
	auto resouce = ShareData::GetInstance().GetDeviceResources();
	RECT windowSize = resouce->GetOutputSize();

	std::wstring wFirstPath = L"Resources/Shader/PostProcess";

	const wchar_t* wLastPath[3] = { L"_VS.cso" , L"_GS.cso" , L"_PS.cso" };

	std::wstring wfilePath[3] = { wFirstPath + wLastPath[0],
								  wFirstPath + wLastPath[1],
								  wFirstPath + wLastPath[2] };

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(wfilePath[0].c_str());
	BinaryFile GSData = BinaryFile::LoadFile(wfilePath[1].c_str());
	BinaryFile PSData = BinaryFile::LoadFile(wfilePath[2].c_str());

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

	//　====================[　シェーダーにデータを渡すためのコンスタントバッファ生成　]
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_cBuffer);

	// プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
	// コモンステートの作成
	m_states = std::make_unique<DirectX::CommonStates>(device);


}

void MyPostProcess::Render(ID3D11ShaderResourceView* const* offscreenTex, ID3D11ShaderResourceView* const* depthTex)
{
	auto context = ShareData::GetInstance().GetContext();
	auto device = ShareData::GetInstance().GetDeviceResources();
	int width = device->GetOutputSize().right;
	int height = device->GetOutputSize().bottom;

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionColorTexture vertex[4] =
	{
		VertexPositionColorTexture(SimpleMath::Vector3(0.0f,0.0f,0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(0.0f, 0.0f)),
	};

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = SimpleMath::Matrix::Identity;
	cbuff.matProj = SimpleMath::Matrix::Identity;
	cbuff.matWorld = SimpleMath::Matrix::Identity;
	cbuff.Diffuse = SimpleMath::Vector4((float)width, (float)height, m_chromaticVal, m_blerVal);

	ShareData::GetInstance().GetCamera()->GetViewMatrix();

	auto camera = ShareData::GetInstance().GetCamera();

	SimpleMath::Viewport viewport = SimpleMath::Viewport();

	SimpleMath::Vector3 eyespos = camera->GetTargetPosition();

	cbuff.eyes = SimpleMath::Vector4(eyespos.x, eyespos.y,eyespos.z,0.0);

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
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);

	//　====================[　カリング：左周り　]
	context->RSSetState(m_states->CullNone());

	//　====================[　シェーダをセット　]
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//　====================[　ピクセルシェーダにテクスチャを登録　]
	context->PSSetShaderResources(0, 1, offscreenTex);

	context->PSSetShaderResources(1, 1, depthTex);

	context->PSSetShaderResources(2, 1, SpriteLoder::GetInstance().GetRampTexture().GetAddressOf());

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
