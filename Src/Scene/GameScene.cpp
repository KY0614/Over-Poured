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
#include "../Object/UI/UIManager.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	player_ = nullptr;
	stage_ = nullptr;
	customer_ = nullptr;
	timer_ = nullptr;
	score_ = 0;
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
	player_->AddCollider(stage_->GetFloorTran().collider);

	//客と注文
	customer_ = std::make_unique<OrderCustomerManager>();
	customer_->Init();

	//タイマー(2:00）２分
	timer_ = std::make_unique<Timer>(MAX_MINUTE_TIME,0);

	//最初のお客の注文を受け取る
	stage_->SetCurrentOrder(customer_->GetOrderData());

	//カメラ
	mainCamera->SetFollow(&player_->GetTransform());
	//mainCamera->ChangeMode(Camera::MODE::FOLLOW);
	mainCamera->ChangeMode(Camera::MODE::TOP_FIXED);

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


#ifdef _DEBUG

	if (stage_->IsServed())
	{
		//スコアの加算
		score_ += customer_->CheckServeAndOrder(stage_->GetServeItems());
		customer_->IsServe();	//注文を出す
		stage_->ResetServeData();	//サーブしたアイテムをリセット
	}

#endif // _DEBUG

	if (timer_->IsEnd())
	{
		scr.SetCurrentScore(score_);
		scr.SaveScore(score_);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}

	//シーン遷移
	if (ins.IsInputTriggered("NextScene"))
	{
		scr.SetCurrentScore(score_);
		scr.SaveScore(score_);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}

	stage_->Update();

	player_->Update();

	customer_->Update();

	timer_->Update();
}

void GameScene::Draw(void)
{
	//ステージ描画
	stage_->Draw();
	//お客と注文描画
	customer_->Draw();
	//プレイヤー描画
	player_->Draw();
	UIManager::GetInstance().Draw();
	//タイマー描画
	timer_->Draw();
}