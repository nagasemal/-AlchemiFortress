#include "MyModelShader.hlsli"

Texture2D ModelTexture : register(t1);
Texture2D NomalTexture : register(t2);

// リムライトの設定
// 視線ベクトルと法線ベクトルが直角に近いほどライトの値を大きくする
float4 ApplyLimLight(float3 normal)
{
	//　リムライト
    float3 invEye = normalize(mul(WorldInverseTranspose, EyePosition));
    float rim = pow(1.0 - clamp(dot(invEye, normal), 0.0, 1.0), 5.0);

    float3 eyedif = normalize(Eyes.xyz - EyePosition);


	// 視線ベクトルとライトベクトルも考慮に入れる
	// pow = 強度増
	// max = 1.0 以上に行かないようにする
    float dotLE_1 = pow(max(dot(eyedif, normalize(LightDirection[0])), 1.0), 30.0);
    float dotLE_2 = pow(max(dot(eyedif, normalize(LightDirection[1])), 1.0), 30.0);
    float dotLE_3 = pow(max(dot(eyedif, normalize(LightDirection[2])), 1.0), 30.0);

    return lerp(0, LimLightColor, rim * dotLE_1 * dotLE_2 * dotLE_3);

}

// フォグの設定
void ApplyFog(inout float4 color, float fogFactor)
{
    color.rgb = lerp(color.rgb, float3(0.5,0.3,0.8) * color.a, fogFactor);
}

float4 ApplyPointLight(float3 position, float3 lightPos,float power,float rage)
{
    // 光源の方向
    float3 lightDirection = position - lightPos;
    
    // 光源までの距離
    float lightDistance = length(lightDirection) * rage;

    // 光源からの距離の影響
    float atten = saturate(1.0f / (lightDistance * lightDistance));
    
    return atten * power;
}

void AddSpecular(inout float4 color, float3 specular)
{
    color.rgb += specular * color.a;
}

float4 main(PSInput input) : SV_TARGET0
{    
    // テクスチャ取得
    float texInput = (float)Texture.Sample(Sampler, input.TexCoord);

	// テクスチャ取得
    float3 modelTexture = ModelTexture.Sample(Sampler, input.TexCoord).rgb;

	// ライトの計算
    input.Normal = normalize(input.Normal);
    
    // マウス周辺のポイントライト
    float4 pointLight_mouse = ApplyPointLight(input.Position.xyz, MousePosition.xyz, 1.0f, 1.0f);
   
    float4 pointLight_crystal = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // クリスタルの周辺を発光させる
    for (int i = 0; i < 3; i++)
    {
        pointLight_crystal += ApplyPointLight(input.Position.xyz, CrystalPosition[i].xyz, 1.0f, 1.5f - cos(Time.w));
    }
	
	// ノーマルマップ取得
    float3 nomalTex = NomalTexture.Sample(Sampler, input.TexCoord).rgb;

    float4 diff = pow(dot(nomalTex, input.Diffuse.rgb + pointLight_crystal.rgb + pointLight_mouse.rgb), 0.5f); /*+ ApplyLimLight(input.Normal)*/

    float4 color = diff;
    
    color.rgb *= PaintColor.rgb;

    // ツヤ消し
    input.Specular *= 0.4f;
    
	// スペキュラーを設定する
    AddSpecular(color, pow(clamp(dot(input.Specular, nomalTex), 0.0f, 1.0f), 1.0f));
    
    color.rgb *= modelTexture.rgb;
    
    color *= texInput ;
    
    // ポイントライト加算
    color += AddScreen(color, float4(0.4, 0.4, 0.1, 0.0f)) * pointLight_mouse;
    
    color += AddScreen(color, float4(0.4, 0.0, 0.4, 0.0f)) * pointLight_crystal;
    
    color.w = 1.0f;
        
    return float4(color.xyz, color.w);
}