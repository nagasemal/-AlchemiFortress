#pragma once

class DrawSlider;
class SelectionBox;

class RenderOption
{
public:
	RenderOption();
	~RenderOption();

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	bool GetHitCancelButton();

private:

	// SEの操作バー
	std::unique_ptr<DrawSlider> m_userBar_SE;
	// BGMの操作バー
	std::unique_ptr<DrawSlider> m_userBar_BGM;
	// SystemSEの操作バー
	std::unique_ptr<DrawSlider> m_userBar_SystemSE;

	// オプション操作を辞める
	std::unique_ptr<SelectionBox> m_cancelButton;

};