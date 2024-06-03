//--------------------------------------------------------------------------------------
// File: ShareData.h
//
// 2Dアニメーションの表示に役に立つ関数をまとめたクラス
// 
// Date: 2023.4.21
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------

#pragma once
#include <SimpleMath.h>

/// <summary>
/// 2Dアニメーションの切り替えに役に立つ関数
/// </summary>
/// <param name="wight">	1画像の横幅</param>
/// <param name="height">	1画像の縦幅</param>
/// <param name="key">		アニメーションを回す順番</param>
/// <param name="beside">	横軸指定</param>
/// <param name="vertical">	行数指定</param>
/// <param name="interval"> 次のアニメーションに切り替わるまでの時間</param>
/// <returns></returns>
RECT SpriteCutter(int wight, int height, int key[], int beside = 0, int vertical = 0, int interval = 60);

/// <summary>
/// スプライトを切るだけの関数
/// </summary>
/// <param name="wight">	1画像の横幅</param>
/// <param name="height">	1画像の縦幅</param>
/// <param name="beside">	横軸指定</param>
///  <param name="vertical">行数指定</param>
/// <returns></returns>
RECT SpriteCutter(int wight, int height, int beside = 0, int vertical = 0);