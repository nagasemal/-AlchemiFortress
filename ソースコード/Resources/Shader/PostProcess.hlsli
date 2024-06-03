cbuffer ConstBuffer : register(b0)
{
    matrix matWorld;
    matrix matView;
    matrix matProj;
    float4 diffuse;
    float4 eyes;
};

// 定数バッファ
cbuffer Parameters : register(b0)
{
    //float3x3 WorldInverseTranspose  : packoffset(c19); // ワールド逆転置行列
    float3 eyePosition              : packoffset(c12); // カメラの位置

};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};