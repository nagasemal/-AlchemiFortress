//--------------------------------------------------------------------------------------
// File: ScreenToWorld.h
//
// スクリーン座標をワールド座標に変換するためのクラス
// 
// Date: 2023.4.19
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once

/// <summary>
/// スクリーン座標から最近点,最遠点を求め、無限XZ平面に衝突していれば
/// その位置を返し、さしていなければ最遠点を返す関数
/// </summary>
/// <param name="sX">スクリーン座標　X</param>
/// <param name="sY">スクリーン座標　Y</param>
/// <param name="screen_W">画面サイズ横幅</param>
/// <param name="screen_H">画面サイズ縦幅</param>
/// <param name="view">ビュー行列</param>
/// <param name="prj">プロジェクション行列</param>
/// <returns></returns>
SimpleMath::Vector3 CalcScreenToXZN(
	int sX,		
	int sY,		
	int screen_W, 
	int screen_H, 
	SimpleMath::Matrix view,
	SimpleMath::Matrix prj
);

/// <summary>
/// スクリーン座標をワールド空間座標にするにあたり必要となる逆行列を
/// 計算し、変換する関数
/// </summary>
/// <param name="sX">スクリーン座標　X</param>
/// <param name="sY">スクリーン座標　Y</param>
/// <param name="fZ">射影空間でのZ値(0~1)</param>
/// <param name="screen_W">画面横幅</param>
/// <param name="screen_H">画面縦幅</param>
/// <param name="view">ビュー行列</param>
/// <param name="prj">プロジェクション行列</param>
/// <returns></returns>
SimpleMath::Vector3 CalcScreenToWorldN(
	int sX,		
	int sY,		
	float fZ,	
	int screen_W, 
	int screen_H, 
	SimpleMath::Matrix view,
	SimpleMath::Matrix prj
);