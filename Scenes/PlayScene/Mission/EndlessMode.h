#pragma once

class Number;

class EndlessMode
{
public:
	EndlessMode();
	~EndlessMode();

	void Initialize();

	void Update();

	void ScoreRender();

	void CreateStage(int wave);

private:

	std::unique_ptr<Number> m_renderScore;

	int m_score;

};