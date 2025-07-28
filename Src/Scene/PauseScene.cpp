#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "PauseScene.h"

PauseScene::PauseScene(SceneManager& manager) :
	update_(&PauseScene::Update),
	draw_(&PauseScene::Draw)
{
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::Init(void)
{

}

void PauseScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsInputTriggered("pause"))
	{
		SceneManager::GetInstance().PopScene();
	}
}

void PauseScene::Draw(void)
{
	DrawString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		L"PAUSE",
		0xFFFFFF);
}
