//--------------------------------------------------------------------------------------
// File: ShaderTemplate.h
//
//　自作シェーダーを簡易的に適応させられるクラス
// 
// Author: KazumaNagase
//--------------------------------------------------------------------------------------

#pragma once

#include "Scenes/Commons/SelectionUI.h"

class ShaderTemplate
{
public:

	//UIのアンカーポイント
	enum ANCHOR
	{
		TOP_LEFT = 0,
		TOP_CENTER,
		TOP_RIGHT,

		MIDDLE_LEFT,
		MIDDLE_CENTER,
		MIDDLE_RIGHT,

		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT

	};

public:
	ShaderTemplate();
	~ShaderTemplate();

	void LoadTexture(const wchar_t* path);

	void Create(const wchar_t* path);
	void LoadShaderFile(const wchar_t* path);
	void AdvanceRender(); // 事前に長い処理は呼んでおく

	void SetWindowSize(const int& width, const int& height)	{ m_windowWidth = width; m_windowHeight = height; };

	void SetScale(SimpleMath::Vector2 scale)				{ m_scale = scale;}
	SimpleMath::Vector2 GetScale()							{ return m_scale; }

	void SetPosition(SimpleMath::Vector2 position)			{ m_position = position;}
	SimpleMath::Vector2 GetPosition()						{ return m_position; }

	void SetColor(SimpleMath::Color color)					{ m_color = color; }
	void SetColor(const DirectX::XMVECTORF32 color)			{ m_color = (SimpleMath::Color)color; }
	SimpleMath::Color GetColor()							{ return m_color; }

	void SetRotationMatrix(SimpleMath::Matrix matrix)		{ m_rotationMatrix = matrix; }

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

protected:

	// テクスチャ情報
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;
	Microsoft::WRL::ComPtr<ID3D11Resource>				m_res;

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

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>		m_geometryShader;

	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;

	SimpleMath::Vector2 m_scale;
	SimpleMath::Vector2 m_baseScale;
	SimpleMath::Vector2 m_position;
	SimpleMath::Color m_color;
	SimpleMath::Matrix	m_rotationMatrix;

};