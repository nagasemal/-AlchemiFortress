#include "Button.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
Texture2D tex3 : register(t2);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// 無地のテクスチャ
	Texture2D none;
	// タイトルロゴ
	float4 output = none.Sample(samLinear,input.Tex);

	// 設定した色を受けとる
	output = Colors;

	return output;
}