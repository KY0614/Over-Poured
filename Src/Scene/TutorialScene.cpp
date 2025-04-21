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
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void TutorialScene::Draw(void)
{
	DrawString(0, 0, "チュートリアル", 0xFFFFFF);
}
