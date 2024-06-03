//--------------------------------------------------------------------------------------
// File: MissionRender.h
//
//　ミッション内容を表示するクラス
// 
// Date: 2023.8.10
// Author: Kazuma Nagase
//--------------------------------------------------------------------------------------
#pragma once
#include "Scenes/PlayScene/UI/Number.h"

class SpriteLoder;
class MissionAnimation;
class DrawMissionLabel;
struct Stage_Data;
struct Stage_Condition;

class MissionRender
{
public:

	MissionRender(SimpleMath::Vector2 pos,SimpleMath::Vector2 rage);
	~MissionRender();

	void Render(Stage_Data data);

	// マシン系のミッション内容を描画
	void Render_MachineMission(std::vector<Stage_Condition> stageData);

	// エネミー系のミッション内容を描画
	void Render_EnemyMission(std::vector<Stage_Condition> stageData);

	// 錬金系のミッション内容を描画
	void Render_AlchemiMission(std::vector<Stage_Condition> stageData);

	// 破壊系のミッション内容を描画
	void Render_DestroyMission(std::vector<Stage_Condition> stageData);

	// 強化系のミッション内容を描画
	void Render_LvUpMission(std::vector<Stage_Condition> stageData);

	// 修繕系のミッション内容を描画
	void Render_RepairMission(std::vector<Stage_Condition> stageData);

	// リソース確保系ミッション内容を描画
	void Render_ResourceMission(std::vector<Stage_Condition> stageData);

	// 拠点Lvのミッション内容を描画
	void Render_BaseLvMission(std::vector<Stage_Condition> stageData);

	// タイマー系のミッション内容を描画
	void Render_TimerMission(std::vector<Stage_Condition> stageData);

	// 行の変数を0にする
	void LineReset();

	// ラインのカウンターを返す
	const int GetLineCount() const  { return m_lineCount; }

	const SimpleMath::Vector2 GetPos()  ;
	const SimpleMath::Vector2 GetRage() ;

	void SetPos(SimpleMath::Vector2 pos) { m_position = pos; }
	void SetRage(SimpleMath::Vector2 rage) { m_rage = rage; }

	// 透明度を設定する
	void SetAlpha(float newAlpha) { m_alpha = newAlpha; }
	// 透明度を返す
	const float GetAlpha() { return m_alpha; }

private:

	void ContentDraw(std::vector<Stage_Condition> stageData, ID3D11ShaderResourceView* nameTex,int purposeNum);
	void LabelDraw(SimpleMath::Vector2 pos);

private:

	std::unique_ptr<Number> m_number;

	SimpleMath::Vector2 m_position;
	SimpleMath::Vector2 m_rage;

	float m_alpha;

	int m_lineCount;
	int m_missionNum;

};