#include "CutTexture.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
Texture2D tex3 : register(t2);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// ルール画像
	float4 output_Rule = tex3.Sample(samLinear, input.Tex);
	// タイトルロゴ
	float4 output = tex.Sample(samLinear, input.Tex);
	// タイトルロゴの柄
	float4 output_2 = tex2.Sample(samLinear, input.Tex);

	// 柄をロゴに付ける
	output.rgb = Colors.rgb;
	output.r = mul(0.25f, diffuse.g);
	output.g = mul(0.25f, diffuse.g);
	output.b = mul(2.0f, diffuse.g);

	output.r *= output_2.a;
	output.g *= output_2.a;
	output.b *= output_2.a;

	// 出現させる
	output.a *= step(output_Rule.x, diffuse.a);

	output.x *= step(output_Rule.x, diffuse.a);
	output.y *= step(output_Rule.y, diffuse.a);
	output.z *= step(output_Rule.z, diffuse.a);

	return output;
}