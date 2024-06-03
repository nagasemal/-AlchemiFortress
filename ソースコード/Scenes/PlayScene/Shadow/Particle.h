//--------------------------------------------------------------------------------------
// File: Particle.h
//
//　パーティクルを発生させます
// 
// Date: 2023.7.3
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include "ParticleUtility.h"
#include <list>

class Particle
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		SimpleMath::Matrix		matWorld;
		SimpleMath::Matrix		matView;
		SimpleMath::Matrix		matProj;
		SimpleMath::Vector4		Diffuse;
	};

	enum EFFECT_TYPE : int
	{
		HIT_BULLET		= 0,		// エネミーに球が当たった
		SPAWN_ENEMY		= 1,		// エネミーが出現した
		DELETE_ENEMY	= 2,		// エネミーが倒れた
		MACHINE_LVUP	= 3,		// マシンのレベルが上がった
		MACHINE_BREAK	= 4,		// マシンが壊された
		MACHINE_SPAWN	= 5,		// マシンが設置さえた際に出るパーティクル
		CLICK			= 6,		// クリックされた際に出るパーティクル
		MINING_EFFECT	= 7,		// 結晶を回収した際に出るパーティクル
		RECOVERY_EFFECT	= 8,		// 魔力を回収した際に出るパーティクル
		DEFENSE_EFFECT	= 9,		// 反射攻撃を行った際に出るパーティクル
		BULLET_LINE		= 10		// バレットの後ろに追従するパーティクル
	};

private:

	//関数
public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	Particle(EFFECT_TYPE type);
	~Particle();

	void LoadTexture(const wchar_t* path);
	void CreateBillboard();

	void Initialize(const wchar_t* path = L"Particle");

	// 常時用
	void Update(SimpleMath::Vector3 pos, bool flag = true,SimpleMath::Color color = { 1.0f,1.0f,1.0f,1.0f });
	// 単発用
	bool OnShot(SimpleMath::Vector3 pos, bool flag, SimpleMath::Color color = {1.0f,1.0f,1.0f,1.0f});

	// パーティクルを動かす/消す処理
	void UpdateParticle();

	void Render();

	// スポーン間隔を設定する
	void SetParticleSpawnTime(float time) { m_particleSpawnTime = time; }

	//変数
private:
	// エフェクトの種類
	EFFECT_TYPE m_effectType;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates>							m_states;

	// テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	m_texture;

	// 頂点配列
	std::vector<DirectX::VertexPositionColorTexture>				m_vertices;

	// パーティクル配列
	std::list<ParticleUtility>										m_particleUtility;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader>						m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader>						m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>					m_geometryShader;

	SimpleMath::Matrix m_billboard;
	SimpleMath::Matrix m_view;
	SimpleMath::Matrix m_proj;

	SimpleMath::Vector3 m_cameraPosition;
	SimpleMath::Vector3 m_cameraTarget;

	// 一度に出すParticleの量
	int m_particleNum;

	// パーティクルの生成間隔
	float m_particleSpawnTime;

	bool m_colFlag;

	float m_timer;

private:

	void CreateShader();

	ParticleUtility CreateEffectParam(EFFECT_TYPE type, SimpleMath::Vector3 pos, SimpleMath::Color color = {1.0f,1.0f,1.0f,1.0f});

};