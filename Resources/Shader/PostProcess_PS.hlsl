#include "PostProcess.hlsli"

Texture2D tex       : register(t0);
Texture2D dTex      : register(t1);
Texture2D rampTex   : register(t2);

SamplerState samLinear : register(s0);

float4 blur(float power,float2 uv)
{
    float center = 1.0f - power * 9.0f;
    float4 output = 0;
    for (int i = 0; i < 9; i++)
    {
        output += tex.Sample(samLinear, uv + float2(((i % 3) - 1) * 0.01f, ((i / 3) - 1) * 0.01f)) * power;
    }
    
    output += tex.Sample(samLinear, uv) * center;
    
    
    return output;
}

float AddOutLine(float power, float2 uv, float offset)
{
    float lineflag = 0.0f;
    
    lineflag -= 8.0f * step(offset, dTex.Sample(samLinear, float2(uv.x, uv.y)).g);
    
    // 八方向サンプリング (厳密には9回)
    for (int i = 0; i < 9; i++)
    {
        // サンプリング位置から指定方向のG成分を受け取る
        float judgement = dTex.Sample(samLinear, uv + float2((((float) i % 3) - 1), (((float)i / 3) -1)) * power).g;
        
        // 満たしていたら加算
        lineflag += step(offset, judgement);
    }
    
    return lineflag;
}

float4 chromaticAberration(float power,float2 uv)
{
    half4 col = tex.Sample(samLinear, uv);
    
    half2 uvBase = uv - 0.5h;
    
    // R値を拡大したものに置き換える    
    half2 uvR = uvBase * (1.0h - power * 2.0h) + 0.5h;
    col.r = tex.Sample(samLinear, uvR).r;
    
    // G値を拡大したものに置き換える
    half2 uvG = uvBase * (1.0h - power) + 0.5h;
    col.g = tex.Sample(samLinear, uvG).g;
    
    
    return col;
}


float4 AddScreen(float4 color,float4 screenCol)
{
    return 1 - (1 - color) * (1 - screenCol);
}

float4 AddOverLay(float4 color, float4 overLayCol)
{
    // オーバーレイの計算に使用する変数を宣言
    half4 ovly_mul = 2 * color * overLayCol; // カラーとオーバーレイの乗算
    half4 ovly_scr = 1 - 2 * (1 - color) * (1 - overLayCol); // スクリーン合成の計算

    // オーバーレイの結果を計算
    half4 ovly = step(color, 0.5) * ovly_mul + (1 - step(color, 0.5)) * ovly_scr;

    // ハイライトの計算（オーバーレイの色が0.5より大きい場合はオーバーレイの結果を使い、そうでない場合は元の色を使う）
    half4 hdlgt = step(overLayCol, 0.5) * ovly_mul + (1 - step(overLayCol, 0.5)) * ovly_scr;

    // ハイライトの色を返す
    return (float4)hdlgt;
}
float4 main(PS_INPUT input) : SV_TARGET
{   
    // 注視点との距離を算出
    float lengthEye = distance(input.Color.xyz,eyePosition);
    
    float4 depthTex = dTex.Sample(samLinear, input.Tex);
    
    // 色収差テクスチャ
    float4 bigBlurTex = chromaticAberration(0.0095 - depthTex.r * 0.0095, input.Tex);
    
    // 明度算出
    float v = max(bigBlurTex.r, max(bigBlurTex.g, bigBlurTex.b));
    
    // 暗度算出
    float v_2 = min(bigBlurTex.r, min(bigBlurTex.g, bigBlurTex.b));
        
    // オーバーレイ   ：   明るい位置に赤み(ややオレンジ)
    bigBlurTex += AddOverLay(bigBlurTex, float4(0.3, 0.35, 0.0, 1.0f)) * step(0.86, v);
    
    // オーバーレイ   ：   暗い位置に青み(やや紫)
    bigBlurTex += AddOverLay(bigBlurTex, float4(0.2, 0.0, 0.45, 1.0f)) * step(v_2, 0.14);
    
    // 乗算           :    青み(やや紫)を乗算
    bigBlurTex = bigBlurTex * (float4(0.98, 0.97, 1.0, 0.001f));
    
    // 距離フォグ
    bigBlurTex += float4(0.25f, 0.25f, 0.3f, 0.5f) * depthTex.b;
    
    // 輪郭の抽出を行う (1.0以上ならばアウトラインとしない)
    float linePower = 0.002f;
    float outlineFlag = step(AddOutLine(linePower, input.Tex, 1.0f), 1.0f);
    
    // 輪郭が存在する場合はテクスチャに0を乗算する
    bigBlurTex *= (float4)outlineFlag;

    return float4(bigBlurTex.xyz,1.0f);
    
}