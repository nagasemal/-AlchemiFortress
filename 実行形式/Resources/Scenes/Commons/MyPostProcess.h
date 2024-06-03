//--------------------------------------------------------------------------------------
// File: MyPostProcess.h
//
//　自作ポストプロセス
// 
// Date: 2024.1.11
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------


#pragma once

class MyPostProcess
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	eyes;
	};

public:
	MyPostProcess();
	~MyPostProcess();

	void CreateShader();
	void Render(ID3D11ShaderResourceView* const* offscreenTex, ID3D11ShaderResourceView* const* depthTex);

	// 画面全体の色収差の強さを決定
	void SetChromaticAberrationVal(float val)	{ m_chromaticVal = val; }
	// 画面全体のブラーの強さを決定
	void SetBlerVal(float val)					{ m_blerVal = val; }

private:

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>		m_geometryShader;

	// コンスタンスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_cBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_inputLayout;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// 頂点情報
	std::vector<DirectX::VertexPositionColorTexture>	m_vertices;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates>				m_states;

	// 色収差の強さ
	float m_chromaticVal;
	// ブラーの強さ
	float m_blerVal;

};