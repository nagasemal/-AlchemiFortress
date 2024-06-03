#include "ZoomUI.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 red = float4(1, 0, 0, 1);
    float4 blue = float4(0, 0, 1, 1);
    float4 green = float4(0, 1, 0, 1);
    
    return red;
    
}