Texture2D Texture : register(t0);
sampler Sampler : register(s0);

// シャドウマップテクスチャ
Texture2D ShadowMapTexture : register(t3);
// シャドウマップ用テクスチャサンプラー
SamplerComparisonState ShadowMapSampler : register(s1);

// 定数バッファ
cbuffer Parameters : register(b0)
{
    float4 DiffuseColor             : packoffset(c0);	// ディフューズ色
    float3 EmissiveColor            : packoffset(c1);	// エミッシブ色
    float3 SpecularColor            : packoffset(c2);	// スペキュラ色
    float  SpecularPower            : packoffset(c2.w);	// スペキュラの強度

    float3 LightDirection[3]        : packoffset(c3);	// ライトの方向
    float3 LightDiffuseColor[3]     : packoffset(c6);	// ライトの色
    float3 LightSpecularColor[3]    : packoffset(c9);	// ライトのスペキュラ色

    float3 EyePosition              : packoffset(c12);	// カメラの位置

    float3 FogColor                 : packoffset(c13);	// フォグの色
    float4 FogVector                : packoffset(c14);	// フォグのかかり具合

    float4x4 World                  : packoffset(c15);	// ワールド行列
    
    float3x3 WorldInverseTranspose  : packoffset(c19);	// ワールド逆転置行列
    float4x4 WorldViewProj          : packoffset(c22);	// ワールド行列 × ビュー行列 × プロジェクション行列

};

// 追加する定数バッファ
cbuffer Parameters : register(b1)
{
    float4x4 LightViewProj : packoffset(c0);
    float4 LightDir : packoffset(c5);
    float4 LightAmbient : packoffset(c6);
};

// プログラム側で変化する時間変数を入れる
cbuffer UserParam : register(b2)
{
    float4 Time;
    float4 PaintColor;
    float4 LimLightColor;
    float4 Eyes;    // 注視点
    float4 LightPosition;
    float4 MousePosition;
    float4 CrystalPosition[10];
    
    float4x4 View; // ビュー行列
    float4x4 Proj; // プロジェクション行列
}

struct PSInput
{
    float4 PositionPS : SV_Position;
    float4 Diffuse : COLOR0;
    float3 Specular : COLOR1;
    float2 TexCoord : TEXCOORD0;
    float4 LightViewPos : TEXCOORD1;
    float4 Position : Position;
    float3 Normal : NORMAL;
};

float4 AddScreen(float4 color, float4 screenCol)
{
    return 1 - (1 - color) * (1 - screenCol);
}