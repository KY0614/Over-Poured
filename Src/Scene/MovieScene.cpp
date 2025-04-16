#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "MovieScene.h"

MovieScene::MovieScene(void)
{
}

MovieScene::~MovieScene(void)
{
}

void MovieScene::Init(void)
{
}

void MovieScene::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
	}

}

void MovieScene::Draw(void)
{
	DrawString(0, 0, "オープニング", 0xFFFFFF);
}
