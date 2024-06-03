#include "TutorialShader.hlsli"

Texture2D backTex : register(t0);
Texture2D textTex : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    
    return float4(1.0f,1.0f,1.0f,1.0);

}