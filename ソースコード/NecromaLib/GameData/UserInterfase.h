//--------------------------------------------------------------------------------------
// File: UserInterface.h
//
// UIをシェーダーを用いて表示するためのクラス
// 
// Date: 2023.5.20
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#include "PrimitiveBatch.h"
#include "DeviceResources.h"

#pragma once

class SpriteLoder;
class TextureData;

class UserInterface
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
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		SimpleMath::Vector4	windowSize;
		SimpleMath::Vector4	color;
	};

	//変数
private:
	DX::DeviceResources* m_pDR;

	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// テクスチャハンドル(中身の描画)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture_2;

	Microsoft::WRL::ComPtr<ID3D11Resource> m_res;

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;

	SimpleMath::Vector2 m_scale;
	SimpleMath::Vector2 m_baseScale;
	SimpleMath::Vector2 m_position;
	SimpleMath::Color m_color;

	// 明暗
	float m_lightValue;

	// 削る方向
	bool m_sharpenW, m_sharpenH;

	// 表示位置
	ANCHOR m_anchor;

	// ratio = 比率,割合
	float m_renderRatio;

	//関数
public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	UserInterface();
	~UserInterface();

	void LoadTexture(const wchar_t* path);
	void LoadTexture(std::wstring path);
	void LoadTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);

	void Create(DX::DeviceResources* pDR
		, const wchar_t* path
		, SimpleMath::Vector2 position
		, SimpleMath::Vector2 scale
		, ANCHOR anchor);

	void Create(DX::DeviceResources* pDR
		, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture
		, SimpleMath::Vector2 position
		, SimpleMath::Vector2 scale
		, ANCHOR anchor);

	void SetPulsTextuer(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex) { m_texture_2 = tex; }

	// XY平面に出す
	void Render();

	//void SetWindowSize(const int& width, const int& height);

	void SetScale(SimpleMath::Vector2 scale);
	SimpleMath::Vector2 GetScale()					{ return m_scale; }
	SimpleMath::Vector2 GetBaseScale()				{ return m_baseScale; }
	void SetPosition(SimpleMath::Vector2 position);
	SimpleMath::Vector2 GetPosition()				{ return m_position; }
	void SetAnchor(ANCHOR anchor);
	ANCHOR GetAnchor()								{ return m_anchor; }

	int GetTexture_W()								{ return m_textureWidth;}
	int GetTexture_H()								{ return m_textureHeight; }

	// 割合
	void SetRenderRatio(float ratio)				{ m_renderRatio = ratio; }
	float GetRenderRatio()							{ return m_renderRatio; }

	// 明暗設定
	void SetValue(float value)						{ m_lightValue = value;}
	float GetValue()								{ return m_lightValue;}

	// 削る方向設定 true = 右 false = 左
	void SetSharpenW(bool direction)				{ m_sharpenW = direction; }
	// 削る方向設定 true = 上 false = 下
	void SetSharpenH(bool direction)				{ m_sharpenH = direction; }

	void SetColor(const SimpleMath::Color color)	{ m_color = color;}
	void SetColor(const DirectX::XMVECTORF32 color) { m_color = (SimpleMath::Color)color; }
	void SetColor(const SimpleMath::Vector3 color, const float alpha) { m_color.R(color.x); m_color.G(color.y); m_color.B(color.z); m_color.A(alpha);}

	SimpleMath::Color GetColor()					{ return m_color;}
	SimpleMath::Vector3 GetColorRGB() { return SimpleMath::Vector3(m_color.R(), m_color.G(), m_color.B()); }

private:

	void CreateShader();

};
