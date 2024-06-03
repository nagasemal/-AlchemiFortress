#pragma once
#include <list>
#include <random>

class ParticleUtility_2D;
struct Particle2D_Pram;
class Particle_2D
{
public:

	// 描画タイプ
	enum ParticleDrawType : int
	{
		DrawNone = 0,
		Add = 1
	};

	// パーティクルの動き方
	enum ParticleMoveType : int
	{
		MoveNone = 0,	// なし
		Brast = 1		// 始点を中心に弾ける
	};

	// 次のパーティクルを出す際の挙動
	enum ParticleNextType : int
	{
		NextNone = 0,	// なし
		Division = 1	// 分裂しながら次のパーティクルを出す
	};

	// パーティクルの線形上の動き方
	enum ParticleLinerType : int
	{
		LinerNone = 0,	// なし
		Point   = 1,	// 各点ごとに保管せずランダムに生成
		Start_F = 2,	// 線形上を始点から終点へなぞるように生成(順転)
		Start_E = 3,	// 線形上を始点から終点へなぞるように生成(反転)
		Chain_F = 4,	// 線形上を全てのポイントを通りながら進む
		Chain_E = 5,	// 線形上を全てのポイントを通りながら進む

	};

	// パーティクルの回転の仕方
	enum ParticleRotateType : int
	{
		RotateNone	= 0,	// なし
		Always		= 1,	// 常に回転
		Direction	= 2,	// 進行方向を向く
	};

	struct LinerEffectData
	{
		// 軌道上を辿るエフェクトを作る際に使用する時間変数
		float timer		= 0.0f;
		// 生成間隔
		float spawnTime = 0.0f;
		// 軌道上を辿るエフェクトのスピード
		float speed = 0.0f;
		// 軌道上を辿るエフェクトの起点となる位置
		SimpleMath::Vector2 savePos = SimpleMath::Vector2();

		// 始点の配列番号
		int first		= 0;
		// 終点の配列番号
		int last		= 0;

		// 現在回数
		int count		= 0;
		int maxCount	= 0;

	};

public:
	Particle_2D();
	~Particle_2D();

	// 単発用
	bool OnShot(std::string tag, SimpleMath::Vector2 pos, bool flag);
	// 常時用
	void Update(std::string tag, SimpleMath::Vector2 pos, bool flag);

	// パーティクルを動かす/消す処理
	void UpdateParticle();
	
	void Render();
	void Render(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,RECT rect);


	const SimpleMath::Vector2 GetPointPos(int index)	{ return m_points[index]; }
	void AddPoint(SimpleMath::Vector2 point)			{ m_points.push_back(point); }
	const std::vector<SimpleMath::Vector2> GetPoints()	{ return m_points;}

	float JudgeVal(float val,float max);
	SimpleMath::Vector2 JudgeVal(SimpleMath::Vector2 val, SimpleMath::Vector2 max);
	SimpleMath::Color JudgeVal(SimpleMath::Color val, SimpleMath::Color max);


private:

	void UpdateLinerEffect(std::string tag);

	ParticleUtility_2D CreateEffectParam(std::string tag, SimpleMath::Vector2 pos);
	ParticleUtility_2D CreateEffectParam(std::string tag, std::list<LinerEffectData>::iterator linerData);

	SimpleMath::Vector2 CreatePosition(Particle2D_Pram pram, SimpleMath::Vector2 pos, float random, std::default_random_engine randomEngine);
	SimpleMath::Vector2 CreatePosition(Particle2D_Pram pram, std::list<LinerEffectData>::iterator linerData, float random, std::default_random_engine randomEngine);
	SimpleMath::Vector2 CreateVelocity(Particle2D_Pram pram, SimpleMath::Vector2 pos, float random);
	SimpleMath::Vector2 CreateAccele(Particle2D_Pram pram, SimpleMath::Vector2 pos, float random);

private:

	// パーティクル配列
	std::list<ParticleUtility_2D>	m_particleUtility;

	// 一度に出すParticleの量
	int m_particleNum;

	// 複数間の点を結ぶために使う
	std::vector<SimpleMath::Vector2> m_points;
	// 軌道上を辿るエフェクトの設定
	std::list<LinerEffectData> m_linerEffectData;

	// パーティクルの生成間隔
	float m_particleSpawnTime;

	float m_timer;

	ParticleDrawType m_drawType;

private:

	////データ受け渡し用コンスタントバッファ(送信側)
	//struct ConstBuffer
	//{
	//	SimpleMath::Matrix		matWorld;
	//	SimpleMath::Matrix		matView;
	//	SimpleMath::Matrix		matProj;
	//	SimpleMath::Vector4		Diffuse;
	//};
	//
	//static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//
	//// バッファ
	//Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	//// 入力レイアウト
	//Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//
	//// プリミティブバッチ
	//std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	////コモンステート
	//std::unique_ptr<DirectX::CommonStates>							m_states;
	//
	//// 頂点シェーダ
	//Microsoft::WRL::ComPtr<ID3D11VertexShader>						m_vertexShader;
	//// ピクセルシェーダ
	//Microsoft::WRL::ComPtr<ID3D11PixelShader>						m_pixelShader;
	//// ジオメトリシェーダ
	//Microsoft::WRL::ComPtr<ID3D11GeometryShader>					m_geometryShader;

};

//　====================[　値の遷移を計算するクラス　]
class ParticleUtility_2D
{
	//変数
private:

	//座標
	SimpleMath::Vector2 m_position;

	//速度
	SimpleMath::Vector2 m_velocity;
	//加速度
	SimpleMath::Vector2 m_accele;

	//スケール
	SimpleMath::Vector2 m_nowScale;
	SimpleMath::Vector2 m_startScale;
	SimpleMath::Vector2 m_endScale;

	//生存時間
	float m_life;
	float m_startLife;

	//ローテーション
	float	m_rotate;
	float	m_startRotate;
	float	m_endRotate;

	//カラー
	SimpleMath::Color m_nowColor;
	SimpleMath::Color m_startColor;
	SimpleMath::Color m_endColor;

	//関数
public:
	/// <summary>
	/// 初期設定
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="pos">初期位置</param>
	/// <param name="velocity">速さ</param>
	/// <param name="accele">加速度</param>
	/// <param name="startScale">初期スケール</param>
	/// <param name="endScale">最終スケール</param>
	/// <param name="startColor">初期カラー</param>
	/// <param name="endColor">最終カラー</param>
	ParticleUtility_2D(
		float life = 1.0f,
		SimpleMath::Vector2 pos = { 0.0f,0.0f},
		SimpleMath::Vector2 velocity = { 0.0f,0.0f},
		SimpleMath::Vector2 accele = { 0.0f,0.0f},
		SimpleMath::Vector2 startScale = { 1.0f,1.0f}, SimpleMath::Vector2 endScale = { 0.0f,0.0f },
		SimpleMath::Color startColor = { 1.0f,1.0f,1.0f,1.0f }, SimpleMath::Color endColor = { 0.0f,0.0f,0.0f,0.0f });
	//デストラクタ
	~ParticleUtility_2D();

	// 更新
	bool Update();

	//getter

	// ポジション
	const SimpleMath::Vector2 GetPosition() { return m_position; }

	// 速さ
	const SimpleMath::Vector2 GetVelocity() { return m_velocity; }

	// 加速度
	const SimpleMath::Vector2 GetAccele() { return m_accele; }

	// 大きさ ： 現在
	const SimpleMath::Vector2 GetNowScale() { return m_nowScale; }
	// 大きさ ： 開始時
	const SimpleMath::Vector2 GetStartScale() { return m_startScale; }
	// 大きさ ： 終了時
	const SimpleMath::Vector2 GetEndScale() { return m_endScale; }

	// 生存時間 : 現在
	const float GetLife() { return m_life; }
	// 生存時間 : 開始時
	const float GetStartLife() { return m_startLife; }

	// 色 : 現在
	const SimpleMath::Color GetNowColor() { return m_nowColor; }
	// 色 : 開始時
	const SimpleMath::Color GetStartColor() { return m_startColor; }
	// 色 : 終了時
	const SimpleMath::Color GetEndColor() { return m_endColor; }

	// 回転	： 現在
	const float GetRad() { return m_rotate; }
	// 回転 : 開始時
	const float GetStateRad() { return m_startRotate; }
	// 回転 : 終了時
	const float GetEndRad() { return m_endRotate; }

	// setter

	// ポジション
	void SetPosition(SimpleMath::Vector2 pos)	{ m_position = pos; }
	// 速さ
	void SetVelocity(SimpleMath::Vector2 vel)	{ m_velocity = vel; }
	// 加速度
	void SetAccele(SimpleMath::Vector2 acc)		{ m_accele = acc; }

	// 大きさ ： 現在
	void SetNowScale(SimpleMath::Vector2 sca)	{ m_nowScale = sca; }
	// 大きさ ： 開始時
	void SetStartScale(SimpleMath::Vector2 sca) { m_startScale = sca; }
	// 大きさ ： 終了時
	void SetEndScale(SimpleMath::Vector2 sca)	{ m_endScale = sca; }

	// 生存時間 ： 現在
	void SetLife(float life)					{ m_life = life; }
	// 生存時間 ： 開始時
	void SetStartLife(float life)				{ m_startLife = life; }

	// 色 ： 現在
	void SetNowColor(SimpleMath::Color col)		{ m_nowColor = col; }
	// 色 ： 開始時
	void SetStartColor(SimpleMath::Color col)	{ m_startColor = col; }
	// 色 ： 終了時
	void SetEndColor(SimpleMath::Color col)		{ m_endColor = col; }

	// 回転角
	void SetRad(float deg)						{ m_rotate = XMConvertToRadians(deg); }
	// 回転 : 開始時
	void SetStateRad(float deg)					{ m_startRotate = XMConvertToRadians(deg); }
	// 回転 : 開始時	
	void SetEndRad(float deg)					{ m_endRotate = XMConvertToRadians(deg); }
};

// パーティクルのStringTo列挙体をサポート
namespace ParticleData_Change
{

	Particle_2D::ParticleMoveType ChangeMoveType(std::string name);

	Particle_2D::ParticleLinerType ChangeLinerType(std::string name);

	Particle_2D::ParticleNextType ChangeNextType(std::string name);

	Particle_2D::ParticleDrawType ChangeDrawType(std::string name);

}