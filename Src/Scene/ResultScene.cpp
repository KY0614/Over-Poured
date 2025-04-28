#include<string>
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Object/Score.h"
#include "ResultScene.h"

ResultScene::ResultScene(void)
{
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
}

void ResultScene::Update(void)
{
	//シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	Score::GetInstance().GetScore();
}

void ResultScene::Draw(void)
{
	DrawString(0, 0, L"リザルト", 0xFFFFFF);

	//スコアを読み込む

	DrawFormatString(0, 20, 0xff0000,L"スコア", 0xFFFFFF);

}
