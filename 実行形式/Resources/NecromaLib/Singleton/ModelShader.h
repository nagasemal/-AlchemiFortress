//--------------------------------------------------------------------------------------
// File: ModelShader.h
//
//　3Dモデルに適応させるシェーダーの設定を保持するシングルトンクラス
// 
// Date: 2023.10.06
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
class ModelShader
{
private:

	ModelShader();
	static ModelShader* instance;

public:
	~ModelShader() = default;

	ModelShader(const ModelShader&) = delete;
	ModelShader& operator=(const ModelShader&) = delete;

	static void Create();
	static void Destroy();

	static ModelShader& GetInstance() { return *instance; }

	/// <summary>
	/// 3Dモデル用のシェーダーを生成します
	/// </summary>
	void CreateModelShader();

	/// <summary>
	/// エフェクト用のシェーダーを生成します
	/// </summary>
	void CreateEffectModel();

	/// <summary>
	/// エフェクト用の時間変数を更新します
	/// </summary>
	/// <param name="deltaTime"></param>
	void Update(float deltaTime);

public:


	/// <summary>
	/// シルエット描画用のデプスステンシルの設定を返します
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetStencilShadow() { return m_depthStencilState_Shadow; }

	/// <summary>
	/// モデルのシルエット用ピクセルシェーダーを返します
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelShadowShader() { return m_modelShadowShader; }

	/// <summary>
	/// モデルの半透明描画用ピクセルシェーダーを返します
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelTransparentShader() { return m_modelTransparentShader; }

	/// <summary>
	/// モデルのエフェクト化ピクセルシェーダーを返します
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelEffectShader() { return m_modelEffect_PS; }
	
	/// <summary>
	/// モデル描画設定に使用するピクセルシェーダーを返します[マシン用]
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetMachineModelShader_PS() { return m_machineModelShader_PS; }

	/// <summary>
	/// モデル描画設定に使用するピクセルシェーダーを返します[汎用]
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetModelMyShader_PS() { return m_modelMyShader_PS; }

	/// <summary>
	/// モデル描画設定に使用するヴァーテックスシェーダーを返します
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetModelMyShader_VS() { return m_modelMyShader_VS; }	

	DirectX::Model* GetMagicTrauabgukarPyram() { return m_magicTrauabgukarPyram.get(); }
	
	/// <summary>
	/// モデル(マシン)の描画設定
	/// </summary>
	/// <param name="color">色情報</param>
	/// <param name="time">時間情報</param>
	/// <param name="texture">テクスチャ情報</param>
	void MachineDrawShader(SimpleMath::Color color,SimpleMath::Vector4 time, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);


	/// <summary>
	/// モデル(汎用)の描画設定
	/// </summary>
	/// <param name="color">色情報</param>
	/// <param name="time">時間情報</param>
	/// <param name="texture">テクスチャ情報</param>
	void ModelDrawShader(SimpleMath::Color color, SimpleMath::Vector4 time, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);

	/// <summary>
	/// モデルエフェクトの描画設定
	/// </summary>
	/// <param name="color">色情報</param>
	/// <param name="time">時間情報</param>
	/// <param name="texture">テクスチャ情報</param>
	void ModelEffectShader(SimpleMath::Color color, SimpleMath::Vector4 time, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);

	/// <summary>
	/// シルエット描画の描画設定
	/// </summary>
	void SilhouetteShader(SimpleMath::Color color = SimpleMath::Color(Colors::LightYellow));

	/// <summary>
	/// 半透明描画の描画設定
	/// </summary>
	void ToransparentShader();

	/// <summary>
	/// オーラ風のエフェクトを描画します
	/// </summary>
	/// <param name="time">揺らめかせる時間変数</param>
	/// <param name="pos">位置</param>
	/// <param name="rage">大きさ</param>
	/// <param name="color">色情報</param>
	void DrawAuraEffect(float time,SimpleMath::Vector3 pos, SimpleMath::Vector3 rage,SimpleMath::Color color = SimpleMath::Color(1.0f,1.0f,1.0f,1.0f));

public:

	/// <summary>
	/// 影描画の描画設定
	/// </summary>
	void ShadowModelDraw(bool outlineFlag = true);

	// シャドウマップ用（レンダーテクスチャ）
	DX::RenderTexture* GetShadowMap() { return m_shadowMapRT.get(); }
	// 深度バッファ用テクスチャ
	Microsoft::WRL::ComPtr<ID3D11Texture2D> GetDepathTexture2D() { return m_depathTexture2D; }
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView() { return m_depathStencilView; }

private:

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Base;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Shadow;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Nomal;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelShadowShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelTransparentShader;

	// 時間経過を伝える追加のコンスタントバッファ
	struct ConstBuffer
	{
		SimpleMath::Vector4 Time;			// 値遷移用時間変数
		SimpleMath::Color PaintColor;		// 追加の色情報
		SimpleMath::Color LimLightColor;	// リムライトのカラー
		SimpleMath::Vector4 eyes;			// 注視点(4の倍数しか受け取れない為、Vector4とする)


		SimpleMath::Vector4 LightPos;

		SimpleMath::Vector4 mousePos;		// ポイントライトの位置

		SimpleMath::Vector4 crystalPos[10];		// クリスタルの位置

		SimpleMath::Matrix view;			// ビュー行列
		SimpleMath::Matrix proj;			// プロジェクション行列
	};


	// マシンモデルに使用するシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_machineModelShader_PS;
	// 通常描画モデルに使用するシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelMyShader_PS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_modelMyShader_VS;

	//　====================[　アウトライン描画に使用　]
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_modelOutLineShader_VS;
	// インプットレイアウト(アウトライン用)
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputOutlineLayout;


	// モデルを用いたエフェクトに用いるシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_modelEffect_PS;

	// インプットレイアウト(通常描画用)
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 受け渡し用のコンスタントバッファ
	D3D11_BUFFER_DESC m_constBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cbuffer;

	// エフェクト用のモデル
	std::unique_ptr<DirectX::Model> m_magicTrauabgukarPyram;

	// エフェクト用のジオメトリックプリミティブ
	std::unique_ptr<DirectX::GeometricPrimitive> m_effectBox;

	// シェーダー用時間変数
	float m_shaderTimer;

	SimpleMath::Vector4 m_lightPos;

private:

	// 定数バッファへのポインタ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_shadowConstantBuffer;

	// 影描画(深度テクスチャに書き込むために)使用するシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_depathShadowShader_PS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_depathShadowShader_VS;

	// シャドウマップ用（レンダーテクスチャ）
	std::unique_ptr<DX::RenderTexture>					m_shadowMapRT;
	// 深度バッファ用テクスチャ
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_depathTexture2D;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_depathStencilView;

	// サンプラー
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			m_shadowMapSampler;

};