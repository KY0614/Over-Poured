#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include"../Utility/StringUtility.h"
#include "SelectScene.h"

SelectScene::SelectScene(void)
{
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
}

void SelectScene::Update(void)
{
	//�V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TUTORIAL);
	}
}

void SelectScene::Draw(void)
{
	DrawString(0, 0, L"��Փx�I��", 0xFFFFFF);
}
