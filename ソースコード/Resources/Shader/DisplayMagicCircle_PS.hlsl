#include "DisplayMagicCircle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//–‚–@w‰æ‘œ•\¦
	float4 output = tex.Sample(samLinear, input.tex) * float4(1,1,1,0.45f);

	return output * input.color;
}