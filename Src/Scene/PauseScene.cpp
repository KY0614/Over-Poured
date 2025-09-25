#include <memory>
#include "../Application.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/GameSystem/SoundManager.h"
#include "../Scene/TitleScene.h"
#include "../Scene/PauseScene/ExplainScene.h"
#include "../Scene/PauseScene/KeyConfigScene.h"
#include "PauseScene.h"

namespace 
{
	//背景画像の大きさ（縦横）
	const int BACK_IMG_SCALE = 1080;
	//出現・消滅アニメーションのフレーム数
	const int APPEAR_INTERVAL = 15;
	//枠の余白
	const int MARGINE_SIZE = 30;
	//メニューリストの高さ
	const int MNEU_LIST_HEIGHT = 216;
	//メニューリストのX座標
	const int MENU_LIST_POS_X = 280;
	//メニューリストの開始Y座標
	const int MENU_START_Y = 200;
	//メニューリストの拡大率
	const float MENU_LIST_SCALE = 0.8f;
	//選択中メニューのマージン(横にずらす量)
	const int SELECT_MENU_MARGINE = 25;
}

PauseScene::PauseScene(void) :
	update_(&PauseScene::UpdateAppear),
	draw_(&PauseScene::DrawProcess)
{
	menuList_ = {
		L"チュートリアル",
		L"キー設定",
		L"ゲームに戻る",
		L"タイトルに戻る",
		L"ゲーム終了"
	};

	menuFuncTable_ = {
	{L"チュートリアル",[this]()
		{
			std::unique_ptr<ExplainScene> scene = std::make_unique<ExplainScene>();
			SceneManager::GetInstance().PushScene(std::move(scene));
		}
	},
	{ L"キー設定",[this]()
		{
			std::unique_ptr<KeyConfigScene> scene = std::make_unique<KeyConfigScene>();
			SceneManager::GetInstance().PushScene(std::move(scene));
		}
	},
	{ L"ゲームに戻る",[this]()
		{
			update_ = &PauseScene::UpdateDisappear;
			draw_ = &PauseScene::DrawProcess;
			return;
		}
	},
	{ L"タイトルに戻る",[this]()
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			return;
		}
	},
	{ L"ゲーム終了",[this]()
		{
			Application::GetInstance().Destroy();
			return;
		}
	}
	};


	backImg_ = -1;
	menuListImg_ = nullptr;
	menuCursorImg_ = -1;
	//リソースの初期化
	auto& resM = ResourceManager::GetInstance();
	resM.InitPause();

	frame_ = 0;
	cursorIdx_ = 0;
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::Init(void)
{
	//サウンドの初期化
	InitSound();
	//画像の読み込み
	LoadImages();
}

void PauseScene::Update(void)
{
	(this->*update_)();
}

void PauseScene::Draw(void)
{
	(this->*draw_)();
}

void PauseScene::UpdateAppear(void)
{
	if (++frame_ >= APPEAR_INTERVAL) {
		update_ = &PauseScene::UpdateNormal;
		draw_ = &PauseScene::DrawNormal;
	}
}

void PauseScene::UpdateDisappear(void)
{
	if (--frame_ <= 0) {
		SceneManager::GetInstance().PopScene();
		return;
	}
}

void PauseScene::DrawProcess(void)
{
	//中心から徐々に広がる
	const Application::Size& wSize = Application::GetInstance().GetWindowSize();
	int centerY = wSize.height_ / 2;		//画面中心Y
	int frameHalfHeight = (wSize.height_ - MARGINE_SIZE * 2) / 2;	//枠の高さの半分(上下に広がるので半分でOK)

	//フェードしつつ0～1までの範囲の値の割合を計算
	//出現・消滅時の高さ変化率(0.0～1.0)
	float rate = static_cast<float>(frame_) /
		static_cast<float>(APPEAR_INTERVAL);
	frameHalfHeight *= rate;
	//背景画像の描画
	DrawExtendGraph(MARGINE_SIZE,
		centerY - frameHalfHeight,
		Application::SCREEN_SIZE_X - MARGINE_SIZE,
		centerY + frameHalfHeight,
		backImg_, true);
}

void PauseScene::UpdateNormal(void)
{	
	auto& sound = SoundManager::GetInstance();
	InputManager& ins = InputManager::GetInstance();
	//ポーズメニューを閉じる
	if (ins.IsInputTriggered("pause"))
	{
		sound.Play(SoundManager::SOUND::MENU_CLOSE);
		update_ = &PauseScene::UpdateDisappear;
		draw_ = &PauseScene::DrawProcess;
		return;
	}
	//メニュー操作
	if (ins.IsInputTriggered("Down"))
	{
		sound.Play(SoundManager::SOUND::NEXT_PAGE);
		cursorIdx_ = (cursorIdx_ + 1) % menuList_.size();
	}
	else if (ins.IsInputTriggered("Up"))
	{
		sound.Play(SoundManager::SOUND::NEXT_PAGE);
		cursorIdx_ = (cursorIdx_ + menuList_.size() - 1) % menuList_.size();
	}
	//決定
	if (ins.IsInputTriggered("Interact"))
	{
		sound.Play(SoundManager::SOUND::RETURN_PAGE);
		auto selectedName = menuList_[cursorIdx_];
		menuFuncTable_[selectedName]();
		return;
	}
}

void PauseScene::DrawNormal(void)
{
	//背景画像の描画
	DrawExtendGraph(MARGINE_SIZE,
		MARGINE_SIZE,
		Application::SCREEN_SIZE_X - MARGINE_SIZE,
		Application::SCREEN_SIZE_Y - MARGINE_SIZE,
		backImg_, true);

	//メニューリストの描画
	DrawMenuList();
}

void PauseScene::DrawMenuList(void)
{
	//画面の大きさに合わせて拡大率を変える
	float scale = static_cast<float>(Application::SCREEN_SIZE_Y) /
		static_cast<float>(Application::SCREEN_MAX_SIZE_Y);

	const int line_start_X = MENU_LIST_POS_X * scale;

	int lineY = MENU_START_Y * scale;

	auto currentStr = menuList_[cursorIdx_];

	for (int i = 0; i < menuList_.size();++i) 
	{
		int lineX = 0;

		//文字列の幅を取得
		int stringWidth = GetDrawStringWidth(menuList_[i].c_str(), menuList_[i].size());

		//カーソルのX座標を動的に計算
		int cursor_X = ((Application::SCREEN_SIZE_X / 2 - line_start_X) - (stringWidth));

		if (menuList_[i] == currentStr)
		{
			DrawRotaGraph(cursor_X ,
				lineY,
				scale, 0.0f, menuCursorImg_, true
			);
			lineX += SELECT_MENU_MARGINE * scale;
		}

		DrawRotaGraph(Application::SCREEN_SIZE_X / 2 + lineX * scale,
			(MENU_START_Y * scale) + (MNEU_LIST_HEIGHT * MENU_LIST_SCALE * scale * i),
			scale * MENU_LIST_SCALE, 0.0f, menuListImg_[i], true
		);

		lineY += MNEU_LIST_HEIGHT * MENU_LIST_SCALE * scale;
	}
}

void PauseScene::LoadImages(void)
{
	//背景画像
	backImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TUTORIAL_BACK).handleId_;
	//メニューリスト画像
	menuListImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::MENU_LIST).handleIds_;
	//メニューカーソル画像
	menuCursorImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::MENU_CURSOR).handleId_;
}

void PauseScene::InitSound(void)
{
	auto& sound = SoundManager::GetInstance();
	//カーソルSE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::NEXT_PAGE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::NEXT_PAGE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::NEXT_PAGE, VOLUME_MAX / 2);
	//決定SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::RETURN_PAGE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::RETURN_PAGE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::RETURN_PAGE, VOLUME_MAX / 2);

	//メニューをとじるSE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::MENU_CLOSE,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::MENU_CLOSE).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::MENU_CLOSE, VOLUME_MAX / 2);
}