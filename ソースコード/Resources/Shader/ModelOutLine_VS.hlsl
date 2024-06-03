#include "MyModelShader.hlsli"

// 頂点シェーダーの入力
struct VSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD;
};

struct ColorPair
{
    float3 Diffuse;
    float3 Specular;
};

struct CommonVSOutput
{
    float4 Pos_ps;
    float4 Diffuse;
    float3 Specular;
    float4 LightViewPos;
    float4 Position;
};

ColorPair ComputeLights(float3 eyeVector, float3 worldNormal, uniform int numLights)
{
    float3x3 lightDirections = 0;
    float3x3 lightDiffuse = 0;
    float3x3 lightSpecular = 0;
    float3x3 halfVectors = 0;

    [unroll]
    for (int i = 0; i < numLights; i++)
    {
        lightDirections[i] = LightDirection[i];
        lightDiffuse[i] = LightDiffuseColor[i];
        lightSpecular[i] = LightSpecularColor[i];

        halfVectors[i] = normalize(eyeVector - lightDirections[i]);
    }

    float3 dotL = mul(-lightDirections, worldNormal);
    float3 dotH = mul(halfVectors, worldNormal);

    float3 zeroL = step(0, dotL);

    float3 diffuse = zeroL * dotL;
    float3 specular = pow(max(dotH, 0) * zeroL, SpecularPower) * dotL;

    ColorPair result;

    result.Diffuse = mul(diffuse, lightDiffuse) * DiffuseColor.rgb + EmissiveColor;
    result.Specular = mul(specular, lightSpecular) * SpecularColor;

    return result;
}

CommonVSOutput ComputeCommonVSOutputWithLighting(float4 position, float3 normal, uniform int numLights)
{
    CommonVSOutput vout;

    float4 pos_ws = mul(position, World);
    float3 eyeVector = normalize(EyePosition - pos_ws.xyz);
    float3 worldNormal = normalize(mul(normal, WorldInverseTranspose));

    ColorPair lightResult = ComputeLights(eyeVector, worldNormal, numLights);

    vout.Pos_ps = mul(position + float4(normalize(worldNormal), 1.0f) * 20.0f / 100, WorldViewProj);
    vout.Diffuse = float4(lightResult.Diffuse, DiffuseColor.a);
    vout.Specular = lightResult.Specular;
    vout.LightViewPos = mul(pos_ws, LightViewProj);
    vout.Position = pos_ws;
    


    return vout;
}

// 逆行列の計算
float3x3 Invert(float3x3 m)
{
    return 1.0 / determinant(m) *
        float3x3(
            m._22 * m._33 - m._23 * m._32, -(m._12 * m._33 - m._13 * m._32), m._12 * m._23 - m._13 * m._22,
            -(m._21 * m._33 - m._23 * m._31), m._11 * m._33 - m._13 * m._31, -(m._11 * m._23 - m._13 * m._21),
            m._21 * m._32 - m._22 * m._31, -(m._11 * m._32 - m._12 * m._31), m._11 * m._22 - m._12 * m._21
        );
}

// DirectXTKの頂点シェーダーをベースに改造した頂点シェーダー（no Fog)
PSInput main(VSInput vin)
{
    PSInput vout;

    CommonVSOutput cout = ComputeCommonVSOutputWithLighting(vin.Position, vin.Normal, 3);
    vout.PositionPS = cout.Pos_ps;
    vout.Diffuse = cout.Diffuse;
    vout.Specular = cout.Specular;
    vout.LightViewPos = cout.LightViewPos;
    vout.TexCoord = vin.TexCoord;
    vout.Position = cout.Position;
    vout.Normal = vin.Normal;
    
    return vout;
}
