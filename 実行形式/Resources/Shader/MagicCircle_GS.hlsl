#include "MagicCircle.hlsli"
static const int vnum = 4;

static const float rage = 0.5f;

static const float4 offset_array[vnum] =
{
	float4(-rage,  rage, 0.0f, 0.0f),	// ¶ã
	float4(rage,  rage, 0.0f, 0.0f),	// ‰Eã
	float4(-rage, -rage, 0.0f, 0.0f),	// ¶‰º
	float4(rage, -rage, 0.0f, 0.0f),	// ‰E‰º
};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{

	for (int i = 0; i < vnum; i++)
	{
		PS_INPUT element;

		float4 res = offset_array[i];

		element.Pos = input[0].Pos + mul(offset_array[i] * input[0].Tex.x, matWorld);

		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);

		element.Color = input[0].Color;

		element.Tex.x = offset_array[i].x + 0.5f;
		element.Tex.y = -offset_array[i].y + 0.5f;

		output.Append(element);
	}

	output.RestartStrip();
}