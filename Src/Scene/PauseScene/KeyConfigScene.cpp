#include "../Application.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "KeyConfigScene.h"

namespace 
{
	//マージンサイズ
	const int MARGINE_SIZE = 30;
	//説明画像のサイズ
	const int EXPLAIN_SIZE = 60;
}

KeyConfigScene::KeyConfigScene(void)
{
	backImg_ = -1;
	keyConfImg_ = -1;
	menuBackImg_ = -1;
	//リソースの初期化
	auto& resM = ResourceManager::GetInstance();
	resM.InitPause();
}

KeyConfigScene::~KeyConfigScene(void)
{
}

void KeyConfigScene::Init(void)
{
	//サウンドの初期化
	InitSound();

	//画像の読み込み
	LoadImages();
}

void KeyConfigScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	auto& sound = SoundManager::GetInstance();
	//メニューへ戻る（スペースキー)
	if (ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::MENU_BACK);
		SceneManager::GetInstance().PopScene();
		return;
	}
}

void KeyConfigScene::Draw(void)
{
	//画面の大きさに合わせて拡大率を変える
	float scale = static_cast<float>(Application::SCREEN_SIZE_Y) /
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);
	//画像の大きさ調整用
	float size = 0.8f;

	//背景
	DrawExtendGraph(MARGINE_SIZE,
		MARGINE_SIZE,
		Application::SCREEN_SIZE_X - MARGINE_SIZE,
		Application::SCREEN_SIZE_Y - MARGINE_SIZE,
		backImg_, true);

	//説明画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 - (EXPLAIN_SIZE * static_cast<int>(scale)),
		scale * size, 0.0f,
		keyConfImg_,
		true);
	//メニューへ戻る画像のずらす量
	const int menuBackImgMargin = 90;
	//メニューへ戻る画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y - MARGINE_SIZE - (
			menuBackImgMargin * static_cast<int>(scale)),
		scale * size, 0.0f,
		menuBackImg_,
		true);
}

void KeyConfigScene::LoadImages(void)
{
	//背景
	backImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TUTORIAL_BACK).handleId_;
	//キー説明画像
	keyConfImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::MENU_KEY_CONFIG).handleId_;
	//メニューへ戻る画像
	menuBackImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::MENU_BACK_LOGO).handleId_;
}

void KeyConfigScene::InitSound(void)
{
	auto& sound = SoundManager::GetInstance();
	//カーソルSE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::MENU_BACK,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::MENU_BACK).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::MENU_BACK, 256 / 2);
}