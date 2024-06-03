#include "MyModelShader.hlsli"

float4 main(PSInput input) : SV_TARGET0
{
	//return float4(0.2f,0.2f, 0.25f, 1.0f);
	
    return PaintColor;
	
    //return float4(0.8f, 0.8f, 0.4f, PaintColor);
}