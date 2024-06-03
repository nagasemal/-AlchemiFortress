#pragma once

// 前方宣言
class TutorialStateContext;
class PlayScene;
class TutorialManager;
class ITutorialState
{
protected:

    TutorialStateContext* m_tutorialContext = nullptr;

public:

    virtual ~ITutorialState() {}

    // チュートリアルコンテキストをセットする
    void Set_Context(TutorialStateContext* context) { this->m_tutorialContext = context; }

    // 更新処理
    virtual void Update(TutorialManager* manager) = 0;

    // 描画処理
    virtual void Render(TutorialManager* manager) = 0;

};

class TutorialStateContext
{

private:
    ITutorialState* m_Tutorialstate;

public:
    TutorialStateContext(ITutorialState* state) : m_Tutorialstate(nullptr) { this->TransitionTo(state); }
    ~TutorialStateContext() { delete m_Tutorialstate;}

    // 次のステートに移行する
    void TransitionTo(ITutorialState* state) 
    {
        // 生成されている場合に限り行う
        if (this->m_Tutorialstate != nullptr)
         
        // 現在のステートを消去
        delete this->m_Tutorialstate;

        // 新しいステートに入れ替える
        this->m_Tutorialstate = state;
        this->m_Tutorialstate->Set_Context(this);
    }

    void Update(TutorialManager* manager) { this->m_Tutorialstate->Update(manager); }

    void Render(TutorialManager* manager) { this->m_Tutorialstate->Render(manager); }

};