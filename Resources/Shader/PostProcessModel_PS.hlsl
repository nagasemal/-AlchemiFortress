#include "MyModelShader.hlsli"

float4 main(PSInput pin) : SV_TARGET0
{    
    // 注視点の方向
    float3 Direction = pin.Position.xyz - Eyes.xyz;
    
    // 注視点とカメラ位置の距離(離れるほど視点範囲を大きくするため)
    float d = distance(Eyes.xyz, EyePosition);
    
    // 注視点までの距離
    float Distance = length(Direction) / (d * 0.25);
    // 注視点からの距離の影響
    float fixationAtten = saturate(1.0f / (Distance * Distance));
    float power = 1.05;
    
    fixationAtten *= power;
           
    // 深度値
    float depth = (length((EyePosition.xyz - Eyes.xyz) - pin.Position.xyz) / 100);
    
    // 注視点範囲,アウトライン描画,深度値,未設定
    return float4(fixationAtten, Time.x, depth, 1.0f);
    
}