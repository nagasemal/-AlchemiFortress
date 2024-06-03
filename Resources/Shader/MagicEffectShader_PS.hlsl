#include "MyModelShader.hlsli"

Texture2D AuraTexture: register(t0);
Texture2D MaskTexture: register(t1);

float4 main(PSInput input) : SV_TARGET0
{
	float2 texCoord = float2(input.TexCoord.x,input.TexCoord.y - Time.w);

	float4 auraTex = AuraTexture.Sample(Sampler, texCoord);

	float4 maskTex = MaskTexture.Sample(Sampler, input.TexCoord);

    float color = mul(auraTex, maskTex);

	float4 col = color * PaintColor;

	col.w *= 0.9f;

	return col;
}