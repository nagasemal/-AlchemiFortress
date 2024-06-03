#pragma once

enum Direction
{
	LEFT = 1 << 0,			//左に当たっている
	TOP = 1 << 1,			//上に当たっている
	RIGHT = 1 << 2,			//右に当たっている
	BOTTOM = 1 << 3,		//下に当たっている
	FRONT = 1 << 4,			//前に当たっている
	BACK = 1 << 5,			//後に当たっている
	TOP_LEFT = TOP + LEFT,		//左上に当たっている
	TOP_RIGHT = TOP + RIGHT,		//右上に当たっている
	BOTTOM_LEFT = BOTTOM + LEFT,	//左下に当たっている
	BOTTOM_RIGHT = BOTTOM + RIGHT,	//右下に当たっている
	CENTER = LEFT + TOP + RIGHT + BOTTOM		//上下左右の中心に当たっている
};

struct Circle
{
	SimpleMath::Vector3 p;

	float r;

	Circle(SimpleMath::Vector3 ip = SimpleMath::Vector3(), float ir = 0.f) { p = ip, r = ir; }

	Circle(float ix, float iy,float iz, float ir) { p = SimpleMath::Vector3(ix, iy,iz), r = ir; }

};

int AABB_XZ(SimpleMath::Vector3 box1APos, SimpleMath::Vector3 box1BPos, SimpleMath::Vector3 box2APos, SimpleMath::Vector3 box2BPos);

int AABB_XY(SimpleMath::Vector3 box1APos, SimpleMath::Vector3 box1BPos, SimpleMath::Vector3 box2APos, SimpleMath::Vector3 box2BPos);

int AABB_3D(SimpleMath::Vector3 box1APos, SimpleMath::Vector3 box1BPos, SimpleMath::Vector3 box2APos, SimpleMath::Vector3 box2BPos);

bool PointerToBox(SimpleMath::Vector2 screenPos,SimpleMath::Vector2 box1, SimpleMath::Vector2 box2);

/// <summary>
/// 円形状の当たり判定をCircleを用いて判定します。
/// </summary>
/// <param name="gameObjectA"></param>
/// <param name="gameObjectB"></param>
/// <returns></returns>
bool CircleCollider(Circle circle1, Circle circle2);

class GameObjct3D;
/// <summary>
/// 円形状の当たり判定をGameObject3Dを用いて判定します。
/// </summary>
/// <param name="gameObjectA"></param>
/// <param name="gameObjectB"></param>
/// <returns></returns>
bool CircleCollider(GameObjct3D* gameObjectA, GameObjct3D* gameObjectB);

bool PointerToCircle(Circle circle, SimpleMath::Vector3 point);

struct OBB
{
	SimpleMath::Vector3 pos;		//　中心点
	SimpleMath::Vector3 inc[3];	//	各座標軸の傾きを表す方向ベクトル
	float length[3];						//	中心点から面までの長さ * 2
};

bool OBBToOBB(OBB obbA, OBB obbB);

struct Plane
{
	SimpleMath::Vector3 pos;		//中心点
	SimpleMath::Vector3 m_local;	//ローカル座標での法線ベクトル
};

// OBBと平面
bool OBBToPlane(OBB obb, Plane plane,float* len);

// ドット積を求める関数
float Dot3D(SimpleMath::Vector3 a, SimpleMath::Vector3 b);

// クロス積
SimpleMath::Vector3 Cross3D(SimpleMath::Vector3 a, SimpleMath::Vector3 b);

float LenSegOnSeparateAxis(SimpleMath::Vector3 Sep, SimpleMath::Vector3 e1, SimpleMath::Vector3 e2, SimpleMath::Vector3* e3 = 0);