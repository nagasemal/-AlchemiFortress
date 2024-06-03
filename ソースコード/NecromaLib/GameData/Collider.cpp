#include <pch.h>
#include "Collider.h"
#include "GameObject3D.h"

//AABB当たり判定集
int AABB_XZ(SimpleMath::Vector3 box1APos, SimpleMath::Vector3 box1BPos, SimpleMath::Vector3 box2APos, SimpleMath::Vector3 box2BPos)
{
	int ret = 0;
	//当たっているか
	if (box1APos.x <= box2BPos.x && box1BPos.x >= box2APos.x && box1APos.z <= box2BPos.z && box1BPos.z >= box2APos.z)
	{
		//幅と高さの和
		SimpleMath::Vector3 maxLength;
		//(box1の幅) + (box2の幅)
		maxLength.x = (box1BPos.x - box1APos.x) + (box2BPos.x - box2APos.x);
		maxLength.z = (box1BPos.z - box1APos.z) + (box2BPos.z - box2APos.z);

		//box1とbox2の中心
		SimpleMath::Vector3 box1Center;	//box1の中心
		SimpleMath::Vector3 box2Center;	//box2の中心
		box1Center.x = (box1APos.x + box1BPos.x) / 2.0f;
		box1Center.z = (box1APos.z + box1BPos.z) / 2.0f;
		box2Center.x = (box2APos.x + box2BPos.x) / 2.0f;
		box2Center.z = (box2APos.z + box2BPos.z) / 2.0f;

		//中心の差　box2 - box1
		SimpleMath::Vector3 length;
		length.x = (box2Center.x - box1Center.x) / maxLength.x;
		length.z = (box2Center.z - box1Center.z) / maxLength.z;

		int checkFlag = 3;

		if (fabs((double)length.x) >= fabs((double)length.z)) {		//横が長い
			checkFlag = 1;
		}
		else if (fabs((double)length.x) < fabs((double)length.z)) {	//縦が長い
			checkFlag = 2;
		}

		//box2から見てbox1がどの方向にあるか
		if ((checkFlag & 1) && (length.x >= 0)) {	//左
			ret += Direction::LEFT;
		}
		if ((checkFlag & 1) && (length.x <= 0)) {	//右
			ret += Direction::RIGHT;
		}
		if ((checkFlag & 2) && (length.z >= 0)) {	//奥
			ret += Direction::BACK;
		}
		if ((checkFlag & 2) && (length.z <= 0)) {	//前
			ret += Direction::FRONT;
		}

	}
	return ret;
}

int AABB_XY(SimpleMath::Vector3 box1APos, SimpleMath::Vector3 box1BPos, SimpleMath::Vector3 box2APos, SimpleMath::Vector3 box2BPos)
{
	int ret = 0;
	//当たっているか
	if (box1APos.x <= box2BPos.x && box1BPos.x >= box2APos.x && box1APos.y <= box2BPos.y && box1BPos.y >= box2APos.y)
	{
		//幅と高さの和
		SimpleMath::Vector3 maxLength;
		//(box1の幅) + (box2の幅)
		maxLength.x = (box1BPos.x - box1APos.x) + (box2BPos.x - box2APos.x);
		maxLength.z = (box1BPos.y - box1APos.y) + (box2BPos.z - box2APos.y);

		//box1とbox2の中心
		SimpleMath::Vector3 box1Center;	//box1の中心
		SimpleMath::Vector3 box2Center;	//box2の中心
		box1Center.x = (box1APos.x + box1BPos.x) / 2.0f;
		box1Center.z = (box1APos.y + box1BPos.y) / 2.0f;
		box2Center.x = (box2APos.x + box2BPos.x) / 2.0f;
		box2Center.z = (box2APos.y + box2BPos.z) / 2.0f;

		//中心の差　box2 - box1
		SimpleMath::Vector3 length;
		length.x = (box2Center.x - box1Center.x) / maxLength.x;
		length.z = (box2Center.z - box1Center.z) / maxLength.z;

		int checkFlag = 3;

		if (fabs((double)length.x) >= fabs((double)length.z)) {		//横が長い
			checkFlag = 1;
		}
		else if (fabs((double)length.x) < fabs((double)length.z)) {	//縦が長い
			checkFlag = 2;
		}

		//box2から見てbox1がどの方向にあるか
		if ((checkFlag & 1) && (length.x >= 0)) {	//左
			ret += Direction::LEFT;
		}
		if ((checkFlag & 1) && (length.x <= 0)) {	//右
			ret += Direction::RIGHT;
		}
		if ((checkFlag & 2) && (length.z >= 0)) {	//奥
			ret += Direction::BACK;
		}
		if ((checkFlag & 2) && (length.z <= 0)) {	//前
			ret += Direction::FRONT;
		}

	}
	return ret;
}

int AABB_3D(SimpleMath::Vector3 box1APos, SimpleMath::Vector3 box1BPos, SimpleMath::Vector3 box2APos, SimpleMath::Vector3 box2BPos)
{
	int ret = 0;
	//当たっているか
	if (box1APos.x <= box2BPos.x && box1BPos.x >= box2APos.x
		&& box1APos.z <= box2BPos.z && box1BPos.z >= box2APos.z
		&& box1APos.y <= box2BPos.y && box1BPos.y >= box2APos.y)
	{
		//幅と高さの和
		SimpleMath::Vector3 maxLength;
		//(box1の幅) + (box2の幅)
		maxLength.x = (box1BPos.x - box1APos.x) + (box2BPos.x - box2APos.x);
		maxLength.y = (box1BPos.y - box1APos.y) + (box2BPos.y - box2APos.y);
		maxLength.z = (box1BPos.z - box1APos.z) + (box2BPos.z - box2APos.z);

		//box1とbox2の中心
		SimpleMath::Vector3 box1Center;	//box1の中心
		SimpleMath::Vector3 box2Center;	//box2の中心
		box1Center.x = (box1APos.x + box1BPos.x) / 2.0f;
		box1Center.y = (box1APos.y + box1BPos.y) / 2.0f;
		box1Center.z = (box1APos.z + box1BPos.z) / 2.0f;

		box2Center.x = (box2APos.x + box2BPos.x) / 2.0f;
		box2Center.y = (box2APos.y + box2BPos.y) / 2.0f;
		box2Center.z = (box2APos.z + box2BPos.z) / 2.0f;

		//中心の差　box2 - box1
		SimpleMath::Vector3 length;
		length.x = (box2Center.x - box1Center.x) / maxLength.x;
		length.y = (box2Center.y - box1Center.y) / maxLength.y;
		length.z = (box2Center.z - box1Center.z) / maxLength.z;

		int checkFlag = 3;

		if (fabs((double)length.x) >= fabs((double)length.z)) {		//横が長い
			checkFlag = 1;
		}
		else if (fabs((double)length.x) < fabs((double)length.z)) {	//縦が長い
			checkFlag = 2;
		}

		//box2から見てbox1がどの方向にあるか
		if ((checkFlag & 1) && (length.x >= 0)) {	//左
			ret += Direction::LEFT;
		}
		if ((checkFlag & 1) && (length.x <= 0)) {	//右
			ret += Direction::RIGHT;
		}
		if ((checkFlag & 2) && (length.z >= 0)) {	//奥
			ret += Direction::BACK;
		}
		if ((checkFlag & 2) && (length.z <= 0)) {	//前
			ret += Direction::FRONT;
		}

	}
	return ret;
}

bool PointerToBox(SimpleMath::Vector2 screenPos, SimpleMath::Vector2 box1, SimpleMath::Vector2 box2)
{

	if (box1.x < screenPos.x && box1.y < screenPos.y && box2.x > screenPos.x && box2.y > screenPos.y) return true;

	return false;
}

//円同士の当たり判定
bool CircleCollider(Circle circle1, Circle circle2)
{
	//中心点の差
	SimpleMath::Vector3 lengthVec = circle1.p - circle2.p;
	float centerDifference = sqrtf(lengthVec.x * lengthVec.x + lengthVec.y * lengthVec.y + lengthVec.z * lengthVec.z);

	//半径の和
	float sumRadius = circle1.r + circle2.r;

	//二つの円の中心点の差が半径の和より小さい
	if (centerDifference <= sumRadius)		return true;

	return false;
}

bool CircleCollider(GameObjct3D* gameObjectA, GameObjct3D* gameObjectB)
{

	// 両者のコライダーアクティブがTrueでなければならない
	if (!gameObjectA->GetColliderActive() || !gameObjectB->GetColliderActive()) return false;

	//中心点の差
	SimpleMath::Vector3 lengthVec = gameObjectA->GetCircle().p - gameObjectB->GetCircle().p;
	float centerDifference = sqrtf(lengthVec.x * lengthVec.x + lengthVec.y * lengthVec.y + lengthVec.z * lengthVec.z);

	//半径の和
	float sumRadius = gameObjectA->GetCircle().r + gameObjectB->GetCircle().r;

	//二つの円の中心点の差が半径の和より小さい
	if (centerDifference <= sumRadius)		return true;

	return false;
}

bool PointerToCircle(Circle circle, SimpleMath::Vector3 point)
{
	Circle pointCircle(point, 0.0f);

	return CircleCollider(circle, pointCircle);
}

//OBB当たり判定
bool OBBToOBB(OBB obb1, OBB obb2)
{

	SimpleMath::Vector3 NAe1 = obb1.inc[0], Ae1 = NAe1 * obb1.length[0];
	SimpleMath::Vector3 NAe2 = obb1.inc[1], Ae2 = NAe2 * obb1.length[1];
	SimpleMath::Vector3 NAe3 = obb1.inc[2], Ae3 = NAe3 * obb1.length[2];
	SimpleMath::Vector3 NBe1 = obb2.inc[0], Be1 = NBe1 * obb2.length[0];
	SimpleMath::Vector3 NBe2 = obb2.inc[1], Be2 = NBe2 * obb2.length[1];
	SimpleMath::Vector3 NBe3 = obb2.inc[2], Be3 = NBe3 * obb2.length[2];

	SimpleMath::Vector3 Interval = obb1.pos - obb2.pos;


	//=[分離軸]=/
		//Ae1
	float rA = Ae1.Length();
	float rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, &Be3);
	float L = fabs(Dot3D(Interval, NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

	// 分離軸 : Ae2
	rA = Ae2.Length();
	rB = LenSegOnSeparateAxis(NAe2,Be1, Be2, &Be3);
	L = fabs(Dot3D(Interval, NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = (Ae3.Length());
	rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, &Be3);
	L = fabs(Dot3D(Interval, NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, &Ae3);
	rB = (Be1.Length());
	L = fabs(Dot3D(Interval, NBe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, &Ae3);
	rB = (Be2.Length());
	L = fabs(Dot3D(Interval, NBe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(NBe3, Ae1,Ae2, &Ae3);
	rB = (Be3.Length());
	L = fabs(Dot3D(Interval, NBe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : C11
	SimpleMath::Vector3 Cross;
	Cross = Cross3D(NAe1, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Dot3D(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	Cross = Cross3D(NAe1,NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Dot3D(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	Cross = Cross3D(NAe1, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Dot3D(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	Cross = Cross3D(NAe2, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Dot3D(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	Cross = Cross3D(NAe2, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Dot3D(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	Cross = Cross3D(NAe2,NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Dot3D(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	Cross = Cross3D(NAe3,NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Dot3D(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	Cross = Cross3D(NAe3, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Dot3D(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	Cross = Cross3D(NAe3,NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Dot3D(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離平面が存在しないので「衝突している」
	return true;
}

bool OBBToPlane(OBB obb, Plane plane,float* len)
{

	float r = 0.0f;								//近接距離

	for (int i = 0; i < 3; i++)
	{
		SimpleMath::Vector3 direct		= obb.inc[i];
		r += fabs(Dot3D((direct * obb.length[i]), plane.m_local));
	}

	float s = Dot3D(obb.pos - plane.pos, plane.m_local);

	// 押し戻し処理
	if (len != NULL)
	{
		if (s > 0) *len = r - fabs(s);

		else	   *len = r + fabs(s);
	}

	if (fabs(s) - r < 0.0f) return true;

	return false;
}

// OBB系で用いた処理達
float Dot3D(SimpleMath::Vector3 a, SimpleMath::Vector3 b)
{
		return a.x * b.x + a.y * b.y + a.z * b.z;
}

SimpleMath::Vector3 Cross3D(SimpleMath::Vector3 a, SimpleMath::Vector3 b)
{
		SimpleMath::Vector3 closs = SimpleMath::Vector3::Zero;

		closs.x = (a.y * b.z - a.z * b.y);
		closs.y = (a.z * b.x - a.x * b.z);
		closs.z = (a.x * b.y - a.y * b.x);

		return closs;
}

float LenSegOnSeparateAxis(SimpleMath::Vector3 Sep, SimpleMath::Vector3 e1, SimpleMath::Vector3 e2, SimpleMath::Vector3* e3)
{
		// 3つの内積の絶対値の和で投影線分長を計算
		// 分離軸Sepは標準化されていること
		float r1 = fabs(Dot3D(Sep, e1));
		float r2 = fabs(Dot3D(Sep, e2));
		float r3 = e3 ? (fabs(Dot3D(Sep, *e3))) : 0;
		return r1 + r2 + r3;
}