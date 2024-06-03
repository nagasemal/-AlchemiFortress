#include "pch.h"
#include "Easing.h"
#include <math.h>

#define PI		(3.14159265359)

float Easing::Lerp(float a, float b, float t)
{
	float p;
	p = b * t + a * (1.f - t);

	return p;
}

int Easing::Lerp(int a, int b, float t)
{
	float p;
	p = Lerp((float)a, (float)b, t);

	return (int)p;
}

SimpleMath::Vector3 Easing::Lerp(SimpleMath::Vector3 a, SimpleMath::Vector3 b, float time)
{
	SimpleMath::Vector3 p;

	p.x = Lerp(a.x, b.x, time);
	p.y = Lerp(a.y, b.y, time);
	p.z = Lerp(a.z, b.z, time);

	return p;
}

SimpleMath::Vector3 Easing::SLerp(SimpleMath::Vector3 a, SimpleMath::Vector3 b, float time)
{
	SimpleMath::Vector3 out, s, e;

	s.Normalize(a);
	e.Normalize(b);

	float angle = acosf(a.Dot(b));

	float SinTh = sin(angle);

	// ï‚ä‘åWêî
	float Ps = sin(angle * (1 - time));
	float Pe = sin(angle * time);

	out = (Ps * s + Pe * e) / SinTh;

	return out;
}

SimpleMath::Matrix Easing::CalcLookAt(SimpleMath::Matrix matrix, SimpleMath::Vector3 pos, SimpleMath::Vector3 look, SimpleMath::Vector3 up)
{
	SimpleMath::Vector3 X, Y, Z, D;
	D = look - pos;
	D.Normalize();

	Y.Normalize(up);

	X.Cross(Y,D);
	X.Normalize();

	Z.Cross(X,Y);
	Z.Normalize();

	matrix._11 = X.x;	matrix._12 = X.y;	matrix._13 = X.z;	matrix._14 = 0;
	matrix._21 = Y.x;	matrix._22 = Y.y;	matrix._23 = Y.z;	matrix._24 = 0;
	matrix._31 = Z.x;	matrix._32 = Z.y;	matrix._33 = Z.z;	matrix._34 = 0;
	matrix._41 = 0.0f;	matrix._42 = 0.0f;	matrix._43 = 0.0f;	matrix._44 = 1.0f;

	return matrix;
}

float Easing::LookAt(SimpleMath::Vector3 a, SimpleMath::Vector3 b)
{
	float sb, sbx, sbz, ax, az, bx, bz = 0.f;

	bx = b.x / 2;
	bz = b.z / 2;

	ax = a.x / 2;
	az = a.z / 2;

	sbx = ax - bx;
	sbz = az - bz;

	sb = sqrtf(sbx * sbx + sbz * sbz);

	return sb;
}

float Easing::Angle(float a1, float a2, float b1, float b2)
{

	float r = atan2f(b2 - a2, b1 - a1);

	if (r < 0)
	{
		r = r + 2.f * (float)PI;
	}
	return floorf(r * 180 / (2.f * (float)PI));
}

#pragma region Sine


float Easing::EaseInSine(float a, float b, float t)
{
	t = 1.f - cosf((t * (float)PI) / 2.f);

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseOutSine(float a, float b, float t)
{
	t = sinf((t * (float)PI) / 2.f);

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseInOutSine(float a, float b, float t)
{
	t = -(cosf((float)PI * t) - 1.f) / 2.f;

	float p;
	p = Lerp(a, b, t);

	return p;
}

#pragma endregion

#pragma region Quad

float Easing::EaseInQuad(float a, float b, float t)
{
	t *= t;

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseOutQuad(float a, float b, float t)
{
	t = 1.f - (1.f - t) * (1.f - t);

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseInOutQuad(float a, float b, float t)
{
	t = t < 0.5f ? 2.f * t * t : 1.f - powf(-2.f * t + 2.f, 2.f) / 2.f;

	float p;
	p = Lerp(a, b, t);

	return p;
}

#pragma endregion

#pragma region Cubic

float Easing::EaseInCubic(float a, float b, float t)
{
	t = t * t * t;

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseOutCubic(float a, float b, float t)
{
	t = 1.f - powf(1.f - t, 3.f);

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseInOutCubic(float a, float b, float t)
{
	t = t < 0.5f ? 4.f * t * t * t : 1 - powf(-2.f * t + 2.f, 3.f) / 2.f;

	float p;
	p = Lerp(a, b, t);

	return p;
}

#pragma endregion

#pragma region Quart

float Easing::EaseInQuart(float a, float b, float t)
{
	t = t * t * t * t;

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseOutQuart(float a, float b, float t)
{
	t = 1.f - powf(1.f - t, 5.f);

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseInOutQuart(float a, float b, float t)
{
	t = t < 0.5f ? 16.f * t * t * t * t * t : 1.f - powf(-2.f * t + 2.f, 5.f) / 2.f;

	float p;
	p = Lerp(a, b, t);

	return p;
}

#pragma endregion

#pragma region Quint

float Easing::EaseInQuint(float a, float b, float t)
{
	t = t * t * t * t * t;

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseOutQuint(float a, float b, float t)
{
	t = 1.f - powf(1.f - t, 5.f);

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseInOutQuint(float a, float b, float t)
{
	t = t < 0.5f ? 16.f * t * t * t * t * t : 1.f - powf(-2.f * t + 2.f, 5.f) / 2.f;

	float p;
	p = Lerp(a, b, t);

	return p;
}

#pragma endregion

#pragma region Expo

float Easing::EaseInExpo(float a, float b, float t)
{
	t = StrictEqualityOperator(t, 0.f) ? 0.f : powf(2.f, 10.f * t - 10.f);

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseOutExpo(float a, float b, float t)
{
	t = StrictEqualityOperator(t, 1.f) ? 1.f : 1.f - powf(2.f, -10.f * t);

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseInOutExpo(float a, float b, float t)
{
	t = StrictEqualityOperator(t, 0.f)
		? 0.f
		: StrictEqualityOperator(t, 1.f)
		? 1.f
		: t < 0.5f ? powf(2.f, 20.f * t - 10.f) / 2.f
		: (2.f - powf(2.f, -20.f * t + 10.f)) / 2.f;

	float p;
	p = Lerp(a, b, t);

	return p;
}

#pragma endregion

#pragma region Circ

float Easing::EaseInCirc(float a, float b, float t)
{
	t = 1.f - sqrtf(1.f - powf(t, 2.f));

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseOutCirc(float a, float b, float t)
{
	t = sqrtf(1.f - powf(t - 1.f, 2.f));

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseInOutCirc(float a, float b, float t)
{
	t = t < 0.5f
		? (1.f - sqrtf(1.f - powf(2.f * t, 2.f))) / 2.f
		: (sqrtf(1.f - powf(-2.f * t + 2.f, 2.f)) + 1.f) / 2.f;

	float p;
	p = Lerp(a, b, t);

	return p;
}

#pragma endregion

#pragma region Back

float Easing::EaseInBack(float a, float b, float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.f;

	t = c3 * t * t * t - c1 * t * t;

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseOutBack(float a, float b, float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.f;

	t = 1.f + c3 * powf(t - 1.f, 3.f) + c1 * powf(t - 1.f, 2.f);

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseInOutBack(float a, float b, float t)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	t = t < 0.5f
		? (powf(2.f * t, 2.f) * ((c2 + 1.f) * 2.f * t - c2)) / 2.f
		: (powf(2.f * t - 2.f, 2.f) * ((c2 + 1.f) * (t * 2.f - 2.f) + c2) + 2.f) / 2.f;

	float p;
	p = Lerp(a, b, t);

	return p;
}

#pragma endregion

#pragma region Elastic

float Easing::EaseInElastic(float a, float b, float t)
{
	const float c4 = (2.f * PI) / 3.f;

	t = StrictEqualityOperator(t, 0.f)
		? 0.f
		: StrictEqualityOperator(t, 1.f)
		? 1.f
		: -powf(2.f, 10.f * t - 10.f) * sinf((t * 10.f - 10.75f) * c4);

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseOutElastic(float a, float b, float t)
{
	const float c4 = (2.f * PI) / 3.f;

	t = StrictEqualityOperator(t, 0.f)
		? 0.f
		: StrictEqualityOperator(t, 1.f)
		? 1.f
		: powf(2.f, -10.f * t) * sinf((t * 10.f - 0.75f) * c4) + 1.f;

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseInOutElastic(float a, float b, float t)
{
	const float c5 = (2.f * PI) / 4.5f;

	t = StrictEqualityOperator(t, 0.f)
		? 0.f
		: StrictEqualityOperator(t, 1.f)
		? 1.f
		: t < 0.5f
		? -(powf(2.f, 20.f * t - 10.f) * sinf((20.f * t - 11.125f) * c5)) / 2.f
		: (powf(2.f, -20.f * t + 10.f) * sinf((20.f * t - 11.125f) * c5)) / 2.f + 1.f;

	float p;
	p = Lerp(a, b, t);

	return p;
}

#pragma endregion

#pragma region Bounce

float Easing::EaseInBounce(float a, float b, float t)
{
	t = 1.f - EaseOutBounce(1.f - t);

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseOutBounce(float a, float b, float t)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1.f / d1) {
		t = n1 * t * t;
	}
	else if (t < 2.f / d1) {
		t = n1 * (t -= 1.5f / d1) * t + 0.75f;
	}
	else if (t < 2.5f / d1) {
		t = n1 * (t -= 2.25f / d1) * t + 0.9375f;
	}
	else {
		t = n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}

	float p;
	p = Lerp(a, b, t);

	return p;
}

float Easing::EaseInOutBounce(float a, float b, float t)
{
	t = t < 0.5f
		? (1.f - EaseOutBounce(1.f - 2.f * t)) / 2.f
		: (1.f + EaseOutBounce(2.f * t - 1.f)) / 2.f;

	float p;
	p = Lerp(a, b, t);

	return p;
}

#pragma endregion

float Easing::EaseOutBounce(float t)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1.f / d1) {
		return n1 * t * t;
	}
	else if (t < 2.f / d1) {
		return n1 * (t -= 1.5f / d1) * t + 0.75f;
	}
	else if (t < 2.5f / d1) {
		return n1 * (t -= 2.25f / d1) * t + 0.9375f;
	}
	else {
		return n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}

}

bool Easing::StrictEqualityOperator(float a, float b)
{
	float error = a - b;

	if (fabsf(error) < 0.01f) return true;
	else return false;
}