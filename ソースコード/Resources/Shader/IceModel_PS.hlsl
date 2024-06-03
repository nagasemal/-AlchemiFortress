#include "MyModelShader.hlsli"

Texture2D IceTexture : register(t0);
Texture2D IceNomal : register(t1);


// 氷をモデルに纏わせる
float4 main(PSInput input) : SV_TARGET0
{
	float4 color = float4(0.0f,0.0f,1.0f,1.0f);
	color = lerp(color, IceTexture.Sample(Sampler,input.TexCoord), 1.0f);

	//　リムライト
	float3 invEye = normalize(mul(WorldInverseTranspose, EyePosition));
	float rim = pow(1.0 - clamp(dot(input.Normal, invEye), 0.0, 1.0), 2.0);

	color.rgb += lerp(0,float4(0.8f,0.8f,0.8f,1.0f), rim).rgb;

	return color;
}