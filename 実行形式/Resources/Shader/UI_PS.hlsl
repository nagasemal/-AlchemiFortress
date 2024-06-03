#include "UI.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

#define PI 3.14159265359

float4 main(PS_INPUT input) : SV_TARGET
{
	//　画像表示
	float4 output = tex.Sample(samLinear, input.tex);

	//　板ポリゴンに任意の色を付ける
	float4 outputw = output * colors;

	outputw.rgb *= input.color.rgb;

	//　if文の描き方と同じ 割合で削る
    float sharpen = 0.0f;
	//　左右どちらから削るかのboolの代替
    float directionFlag = step(1.0f, windowSize.z);
	
    sharpen += lerp(1.0f, 0.0f, step(input.color.a, (colors.z * directionFlag) + (input.tex.x + (input.tex.x * -2.0f * directionFlag)))); // 左側
    
    outputw.a *= sharpen;

	return outputw;
}