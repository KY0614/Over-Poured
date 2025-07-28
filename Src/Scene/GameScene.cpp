#include <DxLib.h>
#include "../Common/Easing.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include"../Manager/GameSystem/OrderCustomerManager.h"
#include"../Manager/GameSystem/Timer.h"
#include"../Manager/GameSystem/SoundManager.h"
#include "../Object/Common/Collider.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Player.h"
#include "../Manager/GameSystem//ScoreManager.h"
#include "../Object/UI/UIManager.h"
#include "PauseScene.h"
#include "GameScene.h"

GameScene::GameScene(void) : 
	update_(&GameScene::UpdateCountDown),
	draw_(&GameScene::DrawCountDown)
{
	phase_ = PHASE::COUNT_DOWN;
	player_ = nullptr;
	stage_ = nullptr;
	customer_ = nullptr;
	timer_ = nullptr;
	score_ = 0;
	numbersImgs_ = nullptr;
	countImgs_ = nullptr;
	cntDownIdx_ = 0;
	cntDownTimer_ = 0.0f;
	scale_ = 0.0f;
	sclTime_ = 0.0f;
	timeUpImg_ = 0;
	remainderSE_ = false;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	auto& sound = SoundManager::GetInstance();
	//bgm追加&再生
	sound.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAME,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_BGM).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GAME, 256 / 3);
	sound.Play(SoundManager::SOUND::GAME);

	//カウントダウン時SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::COUNT_DOWN,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::COUNT_DOWN).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::COUNT_DOWN, 256 / 2);

	//スタート時SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GAME_START,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_START).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GAME_START, 256 / 3);

	//タイマー残り３０秒時SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::TIMER,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::TIMER).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::TIMER, 256 / 3);

	//タイマー残り1０秒時SE
	sound.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::TIMER_FAST,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::TIMER_FAST).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::TIMER_FAST, 256 / 3);

	//タイムアップ時SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GAME_FINISH,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_FINISH).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GAME_FINISH, 256 / 3);

	//スコア用画像
	numbersImgs_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SCORE_NUMBER).handleIds_;

	//カウントダウン用画像
	countImgs_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::COUNTDOWN_NUMBER).handleIds_;

	//タイムアップ用画像
	timeUpImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TIME_UP).handleId_;

	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->Init();

	//ステージ
	stage_ = std::make_unique<StageManager>(*player_);
	stage_->Init();

	//ステージのコライダーを追加
	player_->AddCollider(stage_->GetCounterTran().collider);
	int tableNum = stage_->GetTableNum();
	for(int i = 0; i < tableNum; ++i)
	{
		player_->AddCollider(stage_->GetTableTran(i).collider);
	}
	player_->AddCollider(stage_->GetShowCase().collider);
	player_->AddCollider(stage_->GetDustBox().collider);
	player_->AddCollider(stage_->GetFloorTran().collider);

	//客と注文
	customer_ = std::make_unique<OrderCustomerManager>();
	customer_->Init();

	//タイマー(2:00）２分
	timer_ = std::make_unique<Timer>(MAX_MINUTE_TIME, MAX_SECOND_TIME);

	//最初のお客の注文内容を受け取って設定
	stage_->SetCurrentOrder(customer_->GetOrderData());

	//カメラ
	mainCamera->SetFollow(&player_->GetTransform());
	mainCamera->ChangeMode(Camera::MODE::TOP_FIXED);
}

void GameScene::Update(void)
{
	ScoreManager& scr = ScoreManager::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();
	//フェーズごとの更新処理
	switch (phase_)
	{
	case GameScene::PHASE::COUNT_DOWN:
		
		break;
	
	case GameScene::PHASE::GAME:
		UpdateGame();
		break;
	
	case GameScene::PHASE::FINISH:
		//カウントダウンの時と同じようにイージングで画像を拡大させる
		cntDownTimer_++;
		sclTime_ += SceneManager::GetInstance().GetDeltaTime();
		scale_ = Easing::QuintOut(
			static_cast<float>(sclTime_),
			static_cast<float>(1.0f), 0.0f, 2.0f);
		scale_ = std::clamp(abs(scale_), 0.0f, 2.0f);
		if (cntDownTimer_ >= COUNTDOWN_FRAME)
		{
			if (cntDownIdx_ >= MAX_COUNT_DOWN)
			{
				scale_ = 2.0f;
				scr.SetCurrentScore(score_);
				scr.SaveScore(score_);
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
				return;
			}
			cntDownTimer_ = 0;
			cntDownIdx_++;
			sclTime_ = 0.0f;
			scale_ = 0.0f;
		}
		break;
	
	default:
		break;
	}
}

void GameScene::Draw(void)
{
	switch (phase_)
	{
	case GameScene::PHASE::COUNT_DOWN:
		//ステージ描画
		stage_->Draw();
		//プレイヤー描画
		player_->Draw();

		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2 - 50,
			scale_,
			0.0,
			countImgs_[cntDownIdx_],
			true
		);
		break;

	case GameScene::PHASE::GAME:
		DrawGame();
		break;

	case GameScene::PHASE::FINISH:
		DrawGame();
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2 - 50,
			scale_,
			0.0,
			timeUpImg_,
			true
		);
		break;

	default:
		break;
	}

}

void GameScene::UpdateCountDown(void)
{
}

void GameScene::UpdateGame(void)
{
	InputManager& ins = InputManager::GetInstance();
	ScoreManager& scr = ScoreManager::GetInstance();
	UIManager& ui = UIManager::GetInstance();

	customer_->CheckServeAndOrder(stage_->GetServeItems());

	if (customer_->GetIsMoving())
	{
		//最初のお客の注文を受け取る
		stage_->SetCurrentOrder(customer_->GetOrderData());
	}

	if (ins.IsInputTriggered("pause"))
	{
		SceneManager::GetInstance().PushScene(std::make_shared<PauseScene>());
	}

	//注文数分の商品が提供されたら
	if (stage_->IsServed())
	{
		//スコアの加算
		score_ += customer_->GetOrderScore(stage_->GetServeItems());
		customer_->IsServe();		//注文を出す
		stage_->ResetServeData();	//サーブしたアイテムをリセット
	}

	//残り３０秒になったらチクタク音
	if (!remainderSE_ &&
		timer_->GetMinute() <= 0 &&
		timer_->GetSecond() == SECOND_SOUND_TIME)
	{
		SoundManager::GetInstance().Play(SoundManager::SOUND::TIMER);
		remainderSE_ = true;
	}
	//残り１０秒になったら速いチクタク音
	if (remainderSE_ &&
		timer_->GetMinute() <= 0 &&
		timer_->GetSecond() == SECOND_SOUND_TIME_FAST)
	{
		SoundManager::GetInstance().Play(SoundManager::SOUND::TIMER_FAST);
		remainderSE_ = false;
	}

	//タイマーが０になったらぴぴーって鳴らす
	if (timer_->IsEnd())
	{
		SoundManager::GetInstance().Stop(SoundManager::SOUND::TIMER_FAST);
		SoundManager::GetInstance().Play(SoundManager::SOUND::GAME_FINISH);
		phase_ = PHASE::FINISH;
	}

	stage_->Update();

	player_->Update();

	customer_->Update();

	timer_->Update();
}

void GameScene::DrawCountDown(void)
{
}

void GameScene::DrawGame(void)
{
	//お客と注文描画
	customer_->Draw();
	//ステージ描画
	stage_->Draw();

	//プレイヤー描画
	player_->Draw();
	//UI描画
	UIManager::GetInstance().Draw();
	//タイマー描画
	timer_->Draw();
	//スコア描画
	DrawScore(score_);
}

void GameScene::DrawScore(int score)
{
	std::string str = std::to_string(score);
	const int digitWidth = 70;
	for (int i = 0; i < str.size(); ++i)
	{
		//文字列の各桁を数字に変換して描画
		char ch = str[i];
		if ('0' <= ch && ch <= '9')
		{
			int digit = ch - '0';
			DrawRotaGraph(
				50 + i * digitWidth, 50,
				0.8f,0.0f,
				numbersImgs_[digit], true);
		}
	}
}
