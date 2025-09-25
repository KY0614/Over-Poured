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

namespace
{
	//ゲーム全体の制限時間
	const int  MAX_MINUTE_TIME = 2;			//分
	const int  MAX_SECOND_TIME = 0;			//秒

	//残り秒数が少なったときにSEを鳴らす用の目安秒数
	const int  SECOND_SOUND_TIME = 30;		//残り30秒でSEを鳴らす(遅い方のSE)
	const int  SECOND_SOUND_TIME_FAST = 10;	//残り10秒でSEを鳴らす(速い方のSE)

	//カウントダウン画像の枚数
	const int  MAX_COUNT_DOWN = 4;			//start, 1, 2, 3の4枚

	//カウントダウン画像を拡大する時間
	const float  CNTDOWN_SCL_MAX_TIME = 1.0f;//拡大にかかる最大時間

	//カウントダウン画像の拡大率
	const float  CNTDOWN_IMG_MAX_SCL = 2.0f;	//最大拡大率

	//スコアポップアップのワールド座標
	const VECTOR SCORE_POP_UP_POS = { 225.0f, 76.0f, 190.0f };

	//カウントダウンを表示する座標
	const int COUNT_DOWN_IMG_POS_X = Application::SCREEN_SIZE_X / 2;		//画面中央
	const int COUNT_DOWN_IMG_POS_Y = Application::SCREEN_SIZE_Y / 2 - 50;	//少し上
	//スコア画像の拡大率
	const float SCORE_IMG_SCALE = 0.8f;	
	const int SCORE_IMG_POS = 50;	//スコア画像の表示位置(XとY)
}

GameScene::GameScene(void) : 
	update_(&GameScene::UpdateCountDown),
	draw_(&GameScene::DrawCountDown)
{
	player_ = nullptr;
	stage_ = nullptr;
	customer_ = nullptr;
	timer_ = nullptr;
	score_ = 0;
	numbersImgs_ = nullptr;
	countImgs_ = nullptr;
	cntDownIdx_ = 0;
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
	//サウンド初期化
	InitSound();
	//画像読み込み
	LoadImages();

	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->Init();

	//ステージ
	stage_ = std::make_unique<StageManager>(*player_);
	stage_->Init();

	//ステージオブジェクトのコライダーを追加
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
	(this->*update_)();
}

void GameScene::Draw(void)
{
	(this->*draw_)();
}

void GameScene::UpdateCountDown(void)
{
	SoundManager& sound = SoundManager::GetInstance();
	//カウントダウン音再生&スタート音再生
	if (sclTime_ == 0.0f &&
		cntDownIdx_ < MAX_COUNT_DOWN - 1)sound.Play(SoundManager::SOUND::COUNT_DOWN);
	else if (sclTime_ == 0.0f &&	
		cntDownIdx_ >= MAX_COUNT_DOWN - 1)sound.Play(SoundManager::SOUND::GAME_START);

	//拡大させる時間を更新
	sclTime_ += SceneManager::GetInstance().GetDeltaTime();

	//カウントダウン画像の拡大率をイージングで滑らかに拡大
	scale_ = Easing::QuintOut(
		sclTime_, CNTDOWN_SCL_MAX_TIME, 0.0f, CNTDOWN_IMG_MAX_SCL);

	//拡大率を制限（0.0f～2.0fまで)
	scale_ = std::clamp(abs(scale_), 0.0f, CNTDOWN_IMG_MAX_SCL);

	//拡大させる時間が拡大最大時間を超えたら
	//カウントダウンのインデックスを進める(カウントダウン画像を変更する)
	if (sclTime_ >= CNTDOWN_SCL_MAX_TIME)
	{
		cntDownIdx_++;
		sclTime_ = 0.0f;
		scale_ = 0.0f;
		//start画像までいったらゲーム開始
		if (cntDownIdx_ >= MAX_COUNT_DOWN)
		{
			update_ = &GameScene::UpdateGame;
			draw_ = &GameScene::DrawGame;
		}
	}
}

void GameScene::UpdateGame(void)
{
	InputManager& ins = InputManager::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();
	//お客の注文と提供された商品をチェック
	customer_->CheckServeAndOrder(stage_->GetServeItems());
	//お客が移動中の間に
	if (customer_->GetIsMoving())
	{
		//最初のお客の注文を受け取る
		stage_->SetCurrentOrder(customer_->GetOrderData());
	}
	//ポーズボタンが押されたら
	if (ins.IsInputTriggered("pause"))
	{
		sound.Play(SoundManager::SOUND::MENU_OPEN);
		//ポーズボタンが押されたらポーズシーンへ遷移
		SceneManager::GetInstance().PushScene(std::make_unique<PauseScene>());
	}

	//ポップアップUIの更新
	UIManager::GetInstance().PopUpUIUpdate();

	//注文数分の商品が提供されたら
	if (stage_->IsServed())
	{
		int addScore = customer_->GetOrderScore(stage_->GetServeItems());
		//スコアの加算
		score_ += addScore;
		customer_->IsServe();		//注文を出す
		//ワールド座標をスクリーン座標に変換してポップアップUIを表示
		VECTOR screenPos = ConvWorldPosToScreenPos(SCORE_POP_UP_POS);
		UIManager::GetInstance().AddPopUpUI(addScore, screenPos);
		stage_->ResetServeData();	//サーブしたアイテムをリセット
	}
	//注文がタイムアウトしたら
	if (customer_->IsTimeOutOrder())
	{
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
		//タイムアップ関数へ移行
		update_ = &GameScene::UpdateFinish;
		draw_ = &GameScene::DrawFinish;
	}
	//ステージ更新
	stage_->Update();
	//プレイヤー更新
	player_->Update();
	//お客と注文更新
	customer_->Update();
	//タイマー更新
	timer_->Update();
}

void GameScene::UpdateFinish(void)
{
	ScoreManager& scr = ScoreManager::GetInstance();

	//カウントダウンの時と同じようにイージングで画像を拡大させる
	//拡大させる時間を更新
	sclTime_ += SceneManager::GetInstance().GetDeltaTime();
	
	//画像の拡大率をイージングで滑らかに拡大
	scale_ = Easing::QuintOut(
		sclTime_, CNTDOWN_SCL_MAX_TIME, 0.0f, CNTDOWN_IMG_MAX_SCL);
	
	//拡大率を制限（0.0f～2.0fまで)
	scale_ = std::clamp(abs(scale_), 0.0f, CNTDOWN_IMG_MAX_SCL);
	
	//拡大させる時間が拡大最大時間を超えたら
	//スコアを保存してリザルト画面へ遷移
	if (sclTime_ >= CNTDOWN_SCL_MAX_TIME)
	{
		if (cntDownIdx_ >= MAX_COUNT_DOWN)
		{
			scale_ = CNTDOWN_IMG_MAX_SCL;
			scr.SetCurrentScore(score_);	//現在のスコアをセット
			scr.SaveScore(score_);			//スコアをjsonに保存
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
			return;
		}
		cntDownIdx_++;
		sclTime_ = 0.0f;
		scale_ = 0.0f;
	}
}

void GameScene::DrawCountDown(void)
{
	//ステージ描画
	stage_->Draw();
	//プレイヤー描画
	player_->Draw();

	//カウントダウン画像を拡大して描画
	DrawRotaGraph(
		COUNT_DOWN_IMG_POS_X,
		COUNT_DOWN_IMG_POS_Y,
		scale_,
		0.0,
		countImgs_[cntDownIdx_],
		true
	);
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

void GameScene::DrawFinish(void)
{
	//ゲーム中の描画
	DrawGame();
	
	//タイムアップ画像を拡大して描画
	DrawRotaGraph(
		COUNT_DOWN_IMG_POS_X,
		COUNT_DOWN_IMG_POS_Y,
		scale_,
		0.0,
		timeUpImg_,
		true
	);
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
				SCORE_IMG_POS + i * digitWidth, SCORE_IMG_POS,
				SCORE_IMG_SCALE,0.0f,
				numbersImgs_[digit], true);
		}
	}
}

void GameScene::LoadImages(void)
{
	//スコア用画像
	numbersImgs_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SCORE_NUMBER).handleIds_;

	//カウントダウン用画像
	countImgs_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::COUNTDOWN_NUMBER).handleIds_;

	//タイムアップ用画像
	timeUpImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TIME_UP).handleId_;
}

void GameScene::InitSound(void)
{
	auto& sound = SoundManager::GetInstance();
	//bgm追加&再生
	sound.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAME,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_BGM).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GAME, VOLUME_MAX / 3);
	sound.Play(SoundManager::SOUND::GAME);

	//カウントダウン時SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::COUNT_DOWN,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::COUNT_DOWN).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::COUNT_DOWN, VOLUME_MAX / 2);

	//スタート時SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GAME_START,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_START).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GAME_START, VOLUME_MAX / 3);

	//タイマー残り３０秒時SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::TIMER,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::TIMER).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::TIMER, VOLUME_MAX / 3);

	//タイマー残り1０秒時SE
	sound.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::TIMER_FAST,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::TIMER_FAST).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::TIMER_FAST, VOLUME_MAX / 3);

	//タイムアップ時SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GAME_FINISH,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_FINISH).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GAME_FINISH, VOLUME_MAX / 3);
	
	//メニュー時SE
	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::MENU_OPEN,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::MENU_OPEN).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::MENU_OPEN, VOLUME_MAX / 2);

}