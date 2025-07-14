#include <DxLib.h>
#include "../Application.h"
#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include"../Manager/GameSystem/OrderCustomerManager.h"
#include"../Manager/GameSystem/Timer.h"
#include"../Object/Customer/CustomerBase.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/StageObject.h"
#include"../Object/Order/OrderManager.h"
#include "../Object/Player.h"
#include "../Manager/GameSystem//ScoreManager.h"
#include "../Object/SkyDome.h"
#include "GameScene.h"
namespace {
	bool stop = true;
}

GameScene::GameScene(void)
{
	player_ = nullptr;
	skyDome_ = nullptr;
	stage_ = nullptr;
	customer_ = nullptr;
	timer_ = nullptr;
	score_ = 0;
	time_ = 0.0f;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
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

	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	//客と注文
	customer_ = std::make_unique<OrderCustomerManager>();
	customer_->Init();

	//タイマー
	timer_ = std::make_unique<Timer>();

	//最初のお客の注文を受け取る
	stage_->SetCurrentOrder(customer_->GetOrderData());

	//カメラ
	mainCamera->SetFollow(&player_->GetTransform());
	//mainCamera->ChangeMode(Camera::MODE::FOLLOW);
	mainCamera->ChangeMode(Camera::MODE::TOP_FIXED);

	//タイマーの設定
	time_ = MAX_TIME;
}

void GameScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	ScoreManager& scr = ScoreManager::GetInstance();

	if (customer_->GetIsMoving())
	{
		//最初のお客の注文を受け取る
		stage_->SetCurrentOrder(customer_->GetOrderData());
	}

	if(ins.IsInputTriggered("pause"))
	{
		//stop = !stop;
		score_ += 100;
	}

	if(stop)
	{
		time_ -= SceneManager::GetInstance().GetDeltaTime();
	}

#ifdef _DEBUG

	if (stage_->IsServed())
	{
		//スコアの加算
		score_ += customer_->CheckServeAndOrder(stage_->GetServeItems());
		customer_->IsServe();	//注文を出す
		stage_->ResetServeData();	//サーブしたアイテムをリセット
	}

	//if (timer_ < 0.0f)
	//{
	//	scr.SaveScore(score_);
	//	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	//}

#endif // _DEBUG

	//if (timer_ <= 0.0f)
	//{
	//	scr.SetCurrentScore(score_);
	//	scr.SaveScore(score_);
	//	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	//}

	//シーン遷移
	if (ins.IsInputTriggered("NextScene"))
	{
		scr.SetCurrentScore(score_);
		scr.SaveScore(score_);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}

	skyDome_->Update();

	stage_->Update();

	player_->Update();

	customer_->Update();

	timer_->Update();
}

void GameScene::Draw(void)
{
#ifdef _DEBUG

	DebugDraw();

#endif // _DEBUG

	//背景
	//skyDome_->Draw();

	stage_->Draw();

	customer_->Draw();

	player_->Draw();

	timer_->Draw();
}

void GameScene::DebugDraw(void)
{
	////グリッド線描画
	//VECTOR sPos;
	//VECTOR ePos;
	//float LEN = 1200.0f;
	//float HLEN = LEN / 2.0f;
	//float num = 0.0f;
	//for (int z = -10; z < 10; z++)
	//{
	//	num = static_cast<float>(z);
	//	sPos = { -HLEN, 0.0f, num * 50.0f };
	//	ePos = { HLEN, 0.0f, num * 50.0f };
	//	DrawLine3D(sPos, ePos, 0xff0000);
	//}

	//for (int x = -12; x < 12; x++)
	//{
	//	num = static_cast<float>(x);
	//	sPos = { num * 50.0f, 0.0f, -HLEN };
	//	ePos = { num * 50.0f, 0.0f,  HLEN };
	//	DrawLine3D(sPos, ePos, 0x0000FF);
	//}

	//sPos = { 0.0f,  -HLEN, 0.0f };
	//ePos = { 0.0f, HLEN, 0.0f };
	//DrawLine3D(sPos, ePos, 0x00FF00);
	//
	int line = 2;	//行
	int lineHeight = 50;	//行
	SetFontSize(24);

	//左上から
	//DebugDrawFormat::FormatString(L"                                     time : %2.f", timer_, line);
	int width = GetDrawStringWidth(L"time : %2.f", time_);
	DrawFormatString(Application::SCREEN_SIZE_X / 2 - width /2, 0, 0xffffff, L"time : %2.f秒", time_);
	DrawFormatString(Application::SCREEN_SIZE_X / 2 - width / 2, 32, 0xffffff, L"score : ￥%d", score_);
	//DebugDrawFormat::FormatStringRight(L"score : %d", score_, line, lineHeight);

	SetFontSize(16);
}
