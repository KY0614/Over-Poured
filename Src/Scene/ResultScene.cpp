#include<string>
#include "../Common/DebugDrawFormat.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Score.h"
#include "ResultScene.h"

ResultScene::ResultScene(void)
{
	score_ = nullptr;
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
	score_ = std::make_unique<Score>();
	score_->Init();
}

void ResultScene::Update(void)
{
	score_->Update();
}

void ResultScene::Draw(void)
{
	score_->Draw();

#ifdef _DEBUG
	DebugDraw();
#endif // _DEBUG

}

void ResultScene::DebugDraw(void)
{
}