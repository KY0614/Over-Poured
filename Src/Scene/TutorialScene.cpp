#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene(void)
{
}

TutorialScene::~TutorialScene(void)
{
}

void TutorialScene::Init(void)
{
}

void TutorialScene::Update(void)
{
	//シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void TutorialScene::Draw(void)
{
	DrawString(0, 0, L"チュートリアル", 0xFFFFFF);


	SetFontSize(32);
	int width = GetDrawStringWidth(L"Push Enter", 10);
	DrawString(Application::SCREEN_SIZE_X / 2 - width / 2, 300, L"Push Enter", 0xffffff);
	SetFontSize(16);
}
