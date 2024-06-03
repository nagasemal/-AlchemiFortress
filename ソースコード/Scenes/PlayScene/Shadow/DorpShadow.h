//--------------------------------------------------------------------------------------
// File: DorpShadow.h
//
//　オブジェクトの真下にシェーダーを用いて丸影を落とします
// 
// Date: 2023.7.3
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once

class DorpShadow
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		SimpleMath::Matrix		m_matWorld;
		SimpleMath::Matrix		m_matView;
		SimpleMath::Matrix		m_matProj;
		SimpleMath::Vector4		m_diffuse;
	};

public:
	DorpShadow();
	~DorpShadow();

	void Initialize();
	void Update();
	void Render();
	void Finalize();
	void CreateShader();

	// vector配列に中身を入れる
	void CreateShadow(SimpleMath::Vector3 pos);
	// vector配列の中身を消去する
	void DeleteShadow();

	void CreateWorld();

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

private:

	// 丸影テクスチャ情報
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// コンスタンスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// 頂点情報
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	SimpleMath::Matrix m_world;
	SimpleMath::Matrix m_view;
	SimpleMath::Matrix m_proj;

};