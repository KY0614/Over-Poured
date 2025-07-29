#include "../Application.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "PauseScene.h"

PauseScene::PauseScene(void) :
	update_(&PauseScene::UpdateNormal),
	draw_(&PauseScene::DrawNormal)
{
	backImg_ = -1;
	//ƒŠƒ\[ƒX‚Ì‰Šú‰»
	auto& resM = ResourceManager::GetInstance();
	resM.InitPause();
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::Init(void)
{
	backImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TUTORIAL_BACK).handleId_;
}

void PauseScene::Update(void)
{
	(this->*update_)();
}

void PauseScene::Draw(void)
{
	(this->*draw_)();
}

void PauseScene::UpdateNormal(void)
{
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsInputTriggered("pause"))
	{
		SceneManager::GetInstance().PopScene();
	}
}

void PauseScene::DrawNormal(void)
{
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f, 0.0f,
		backImg_,
		true
	);

	SetFontSize(40);
	DrawString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		L"PAUSE",
		0xFFFFFF);
	SetFontSize(16);
}
