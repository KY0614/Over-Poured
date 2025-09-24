#include <DxLib.h>
#include "../Application.h"
#include "../Utility/CommonUtility.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Object/Common/Collider.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Player.h"
#include "../Object/UI/UIManager.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene(void) :
	update_(&TutorialScene::UpdateOperation),
	draw_(&TutorialScene::DrawOperation)
{
	tutorialImgs_ = nullptr;
	tutorialBackImg_ = 0;
	cursorImg_ = nullptr;
	decoImg_ = 0;
	imgIdx_ = 0;
	blinkTime_ = 0;
	blinkIdx_ = 0;
	pushImg_ = 0;

	isBlinkR_ = false;
	isBlinkL_ = false;
	isView_ = false;
	highlightTime_ = 0.0f;
}

TutorialScene::~TutorialScene(void)
{
}

void TutorialScene::Init(void)
{
	ImageInit();
}

void TutorialScene::Update(void)
{
	(this->*update_)();
}

void TutorialScene::Draw(void)
{
	(this->*draw_)();
}

void TutorialScene::UpdateOperation(void)
{
	ImageUpdate();
}

void TutorialScene::DrawOperation(void)
{
	ImageDraw();
}

void TutorialScene::ImageInit(void)
{
	auto& sound = SoundManager::GetInstance();
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::TUTORIAL,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::TUTORIAL_BGM).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::TUTORIAL, 256 / 2);
	sound.Play(SoundManager::SOUND::TUTORIAL);

	//ページを進ませるSE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::NEXT_PAGE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::NEXT_PAGE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::NEXT_PAGE, 256 / 2);

	//ページを戻らせるSE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::RETURN_PAGE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::RETURN_PAGE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::RETURN_PAGE, 256 / 2);

	//ページが進まないSE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::NOT_PAGE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::NOT_PAGE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::NOT_PAGE, 256 / 2);

	//開店カランコロン
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::OPEN_DOOR,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::OPEN_DOOR).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::OPEN_DOOR, 256 / 2);

	//説明用画像読み込み
	tutorialImgs_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TUTORIAL).handleIds_;

	//説明背景画像読み込み
	tutorialBackImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TUTORIAL_BACK).handleId_;

	//カーソル背景画像読み込み
	cursorImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::CURSOR_R).handleIds_;

	//装飾画像読み込み
	decoImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PINK_BANNER).handleId_;

	//画像読み込み
	pushImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PUSH_SPACE).handleId_;

	isBlinkR_ = true;
}

void TutorialScene::ImageUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	auto& sound = SoundManager::GetInstance();
	blinkTime_++;
	blinkIdx_ = (blinkTime_ / 60) % 2;

	//シーン遷移(チュートリアルのページが端までいったら）
	if (isView_ &&
		ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::OPEN_DOOR);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		return;
	}

	//点滅表記
	isBlinkL_ = false;
	isBlinkR_ = false;
	if (imgIdx_ == 0)
	{
		isBlinkR_ = true;
	}
	else if (imgIdx_ > 0 &&
		imgIdx_ < INDEX_MAX)
	{
		isBlinkL_ = true;
		isBlinkR_ = true;
	}
	else {
		isBlinkL_ = true;
		isView_ = true;
	}

	//ページを進ませる
	if (ins.IsInputTriggered("Right"))
	{
		if (imgIdx_ < INDEX_MAX)
		{
			sound.Play(SoundManager::SOUND::NEXT_PAGE);
			imgIdx_++;
		}
		else
		{//ページがないときは違うSE
			imgIdx_ = INDEX_MAX;
			sound.Play(SoundManager::SOUND::NOT_PAGE);
		}
	}
	//ページを戻らせる
	if (ins.IsInputTriggered("Left"))
	{
		if (imgIdx_ > 0)
		{
			sound.Play(SoundManager::SOUND::RETURN_PAGE);
			imgIdx_--;
		}
		else
		{//ページがないときは違うSE
			imgIdx_ = 0;
			sound.Play(SoundManager::SOUND::NOT_PAGE);
		}
	}
}

void TutorialScene::ImageDraw(void)
{
	//画面の大きさに合わせて拡大率を変える
	float scale = static_cast<float>(Application::SCREEN_SIZE_Y) /
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);

	float size = 0.8f;
	//説明画像背景用
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		(int)(540.0f) - 20,
		scale, 0.0f,
		tutorialBackImg_,
		true);
	//装飾（左）
	DrawRotaGraph3(BACK_IMG_MARGINE, BACK_IMG_MARGINE,
		BACK_IMG_SCALE / 2, BACK_IMG_SCALE / 2,
		scale, scale * 2.5f,
		0.0f,
		decoImg_,
		true, false);
	//装飾（右）
	DrawRotaGraph3(
		Application::SCREEN_SIZE_X - BACK_IMG_MARGINE,
		BACK_IMG_MARGINE,
		BACK_IMG_SCALE / 2, BACK_IMG_SCALE / 2,
		scale, scale * 2.5f,
		0.0f,
		decoImg_,
		true, false);
	//説明画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		(int)(540.0f * size),
		scale * size, 0.0f,
		tutorialImgs_[imgIdx_],
		true);
	//点滅表示
	if (isBlinkR_)
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X - CURSOR_IMG_MARGINE,
			Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2,
			scale, 0.0f,
			cursorImg_[blinkIdx_],
			true
		);
	}
	else
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X - CURSOR_IMG_MARGINE,
			Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2,
			scale, 0.0f,
			cursorImg_[1],
			true
		);
	}
	if (isBlinkL_)
	{
		//反転用(左用）
		DrawRotaGraph(CURSOR_IMG_MARGINE,
			Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2,
			scale, 0.0f,
			cursorImg_[blinkIdx_],
			true, true
		);
	}
	else
	{
		//反転用(左用）
		DrawRotaGraph(CURSOR_IMG_MARGINE,
			Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2,
			scale, 0.0f,
			cursorImg_[1],
			true, true
		);
	}

	int logoScl = (int)((float)(LOGO_HEIGHT / 2) * scale);
	if (isView_)
	{
		//pushspaceの画像
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2 + logoScl,
			2.0f, 0.0, pushImg_, true);
	}
}
