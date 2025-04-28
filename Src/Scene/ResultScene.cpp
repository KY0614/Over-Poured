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
	//�V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	Score::GetInstance().GetScore();
}

void ResultScene::Draw(void)
{
	DrawString(0, 0, L"���U���g", 0xFFFFFF);

	//�X�R�A��ǂݍ���

	DrawFormatString(0, 20, 0xff0000,L"�X�R�A", 0xFFFFFF);

}
