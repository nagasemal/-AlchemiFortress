#include "Veil.hlsli"

Texture2D tex : register(t0);
Texture2D ruleTex : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// 設定した色を受けとる
	float4 output = Colors;
	// ルール画像を受け取る
	float4 rule = ruleTex.Sample(samLinear, input.Tex);

	// diffuse.g,bには縦横の値が入っている
	float2 imageRage = float2(input.Tex.x * diffuse.g, input.Tex.y * diffuse.b);
	// 中に表示する画像を受け取る
	float4 imageTex = tex.Sample(samLinear, imageRage);

	// ルール画像を用いたフェード処理 (diffuse.a 時間変数.1)
	output.a *= step(rule.x, diffuse.a);
	// 演出の為,色情報に同じフェード処理を行う　(diffuse.r 時間変数.2)
	output.rgb *= step(rule.x, diffuse.r);
	//// 同じ処理を中に描画する文字画像にも行う
	imageTex.a *= step(rule.x, diffuse.r);

	// 幕にのみ画像を描画する
	output += imageTex;

	return output;
}