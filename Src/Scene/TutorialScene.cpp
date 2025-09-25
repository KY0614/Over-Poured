#include <DxLib.h>
#include "../Application.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "TutorialScene.h"

namespace
{
	//背景画像関連
	const int BACK_IMG_MARGINE = 250;	//初期座標から少しだけ間隔をあける
	const int BACK_IMG_SCALE = 500;		//背景画像の大きさ
	const int CURSOR_IMG_MARGINE = 300;	//初期座標から少しだけ間隔をあける

	//カーソル点滅関連
	const float HIGH_LIGHT_INTERVAL = 1.2f;	//点滅の間隔
	//点滅のフレーム数
	const int BLINK_FRAME = 60;
	//点滅切り替えのフレーム数
	const int BLINK_TOGGLE_FRAME = 2;

	//ロゴ画像の大きさ
	static constexpr int LOGO_HEIGHT = 1024;

	//説明用画像関連
	const int TUTORIAL_IMG_MAX_NUM = 3;		//説明用画像の最大枚数
	const int INDEX_MAX = TUTORIAL_IMG_MAX_NUM - 1;	//説明画像の最大インデックス数(０からなので１引いておく)
}

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
	isPushButton_ = false;
}

TutorialScene::~TutorialScene(void)
{
}

void TutorialScene::Init(void)
{
	//サウンド初期化
	InitSound();
	//画像読み込み
	LoadImages();

	//カーソル点滅初期化
	isBlinkR_ = true;
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
	//カーソル点滅
	CursorHightlight();
	//ページ送り
	ProcessPageChange();
}

void TutorialScene::DrawOperation(void)
{
	//画像描画
	ImageDraw();
}

void TutorialScene::LoadImages(void)
{
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

}

void TutorialScene::InitSound(void)
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
}

void TutorialScene::CursorHightlight(void)
{
	//点滅時間更新
	blinkTime_++;
	//点滅インデックス更新
	blinkIdx_ = (blinkTime_ / BLINK_FRAME) % BLINK_TOGGLE_FRAME;

	//点滅表記
	isBlinkL_ = false;
	isBlinkR_ = false;
	//ページの端によって点滅させるカーソルを変える
	if (imgIdx_ == 0)	//最初のページ
	{
		isBlinkR_ = true;
	}
	else if (imgIdx_ > 0 &&		//途中のページ
		imgIdx_ < INDEX_MAX)
	{
		isBlinkL_ = true;
		isBlinkR_ = true;
	}
	else {				//最後のページ
		isBlinkL_ = true;
		isPushButton_ = true;
	}
}

void TutorialScene::ProcessPageChange(void)
{
	InputManager& ins = InputManager::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();

	//シーン遷移(チュートリアルのページが端までいったら）
	if (isPushButton_ &&
		ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::OPEN_DOOR);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		return;
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
	float aspectRatio = static_cast<float>(Application::SCREEN_SIZE_Y) /
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);
	//説明画像の大きさ
	const float size = 0.8f;
	//装飾の拡大率（Y軸のみ変える）
	const float decoScaleY = 2.5f;
	//説明画像のずらす位置
	const int backImgMarginY = 20;

	//説明画像背景用
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y/2 - backImgMarginY,
		aspectRatio, 0.0f,
		tutorialBackImg_,
		true);
	//装飾（左）
	DrawRotaGraph3(BACK_IMG_MARGINE, BACK_IMG_MARGINE,
		BACK_IMG_SCALE / 2, BACK_IMG_SCALE / 2,
		aspectRatio, aspectRatio * decoScaleY,
		0.0f,
		decoImg_,
		true, false);
	//装飾（右）
	DrawRotaGraph3(
		Application::SCREEN_SIZE_X - BACK_IMG_MARGINE,
		BACK_IMG_MARGINE,
		BACK_IMG_SCALE / 2, BACK_IMG_SCALE / 2,
		aspectRatio, aspectRatio * decoScaleY,
		0.0f,
		decoImg_,
		true, false);
	//説明画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 ,
		aspectRatio * size, 0.0f,
		tutorialImgs_[imgIdx_],
		true);
	//点滅表示
	if (isBlinkR_)
	{
		//右用
		DrawRotaGraph((Application::SCREEN_SIZE_X - CURSOR_IMG_MARGINE),
			(Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2),
			aspectRatio, 0.0f,
			cursorImg_[blinkIdx_],
			true
		);
	}
	else
	{
		//反転用(右用）
		DrawRotaGraph((Application::SCREEN_SIZE_X - CURSOR_IMG_MARGINE),
			(Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2),
			aspectRatio, 0.0f,
			cursorImg_[1],
			true
		);
	}
	if (isBlinkL_)
	{
		//反転用(左用）
		DrawRotaGraph(CURSOR_IMG_MARGINE,
			Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2,
			aspectRatio, 0.0f,
			cursorImg_[blinkIdx_],
			true, true
		);
	}
	else
	{
		//反転用(左用）
		DrawRotaGraph(CURSOR_IMG_MARGINE,
			Application::SCREEN_SIZE_Y - CURSOR_IMG_MARGINE / 2,
			aspectRatio, 0.0f,
			cursorImg_[1],
			true, true
		);
	}
	//pushspaceの位置（画像の大きさを考慮して）
	const int logoPosY = static_cast<int>(static_cast<float>(LOGO_HEIGHT / 2) * aspectRatio);
	//pushspaceの拡大率
	const float logoScale = 2.0f;
	if (isPushButton_)
	{
		//pushspaceの画像
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2 + logoPosY,
			logoScale, 0.0, pushImg_, true);
	}
}
