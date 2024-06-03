#include <pch.h>
#include "ScreenToWorld.h"

SimpleMath::Vector3 CalcScreenToWorldN(int sX, int sY, float fZ, int screen_W, int screen_H, SimpleMath::Matrix view, SimpleMath::Matrix prj)
{
	// 各行列の逆行列を算出
	XMMATRIX invView, invPrj, vP, invViewport = XMMATRIX();

	// ビュー行列の逆行列
	invView = XMMatrixInverse(nullptr, view);

	// プロジェクション行列の逆行列
	invPrj	= XMMatrixInverse(nullptr, prj);

	// ビューポート行列
	vP		= XMMatrixIdentity();

	XMFLOAT4X4 matrix = XMFLOAT4X4();

	// 画面サイズからビューポート行列を算出する
	matrix._11 =  screen_W / 2.f;
	matrix._22 = -screen_H / 2.f;
	matrix._41 =  screen_W / 2.f;
	matrix._42 =  screen_H / 2.f;

	// XMFLOAT4X4をXMMATRIXに変換してvPに入れる
	vP += DirectX::XMLoadFloat4x4(&matrix);
	// ビューポート行列の逆行列
	invViewport = XMMatrixInverse(nullptr, vP);

	// スクリーン座標をワールド空間座標に変換する行列　逆変換
	SimpleMath::Matrix tmp = invViewport * invPrj * invView;

	// スクリーン座標(sX,sY)と最近,遠点(fZ)にtmpをかけてワールド空間座標を算出
	SimpleMath::Vector3 pOut = XMVector3TransformCoord(SimpleMath::Vector3((float)sX, (float)sY, (float)fZ), tmp);

	return pOut;
}

SimpleMath::Vector3 CalcScreenToXZN(int sX, int sY, int screen_W, int screen_H, SimpleMath::Matrix view, SimpleMath::Matrix prj)
{
	SimpleMath::Vector3 nearPos, farPos, ray = SimpleMath::Vector3::Zero;

	// 最近点
	nearPos = CalcScreenToWorldN(sX, sY, 0.0f, screen_W, screen_H, view, prj);

	// 最遠点
	farPos = CalcScreenToWorldN(sX, sY, 1.0f, screen_W, screen_H, view, prj);

	// 距離を取得し、ノーマライズする
	ray = farPos - nearPos;
	ray.Normalize();

	SimpleMath::Vector3 pOut = SimpleMath::Vector3::Zero;

	// カメラから見てYが無限に続くかどうか　交点が存在するかどうか
	if (ray.y <= 0)
	{
		// 床交点
		SimpleMath::Vector3 lRay = XMVector3Dot(ray, SimpleMath::Vector3(0, 1, 0));
		SimpleMath::Vector3 lp0  = XMVector3Dot(-nearPos, SimpleMath::Vector3(0, 1, 0));

		pOut = nearPos + (lp0 / lRay) * ray;

	}
	// 存在しない場合は最遠点とする
	else 		pOut = farPos;

	// マウスのワールド座標
	return pOut;
}