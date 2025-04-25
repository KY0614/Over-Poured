#include <DxLib.h>
#include <math.h>
#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include"../Manager/GameSystem/OrderManager.h"
#include"../Manager/GameSystem/OrderCustomerManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Score.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	player_ = nullptr;
	skyDome_ = nullptr;
	stage_ = nullptr;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	//プレイヤー
	player_ = std::make_shared<Player>();
	player_->Init();

	//ステージ
	stage_ = std::make_unique<Stage>(*player_);
	stage_->Init();

	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	//注文
	order_ = std::make_unique<OrderManager>();
	order_->Init();

	customer_ = std::make_unique<OrderCustomerManager>(*order_);
	customer_->Init();

	//customer_ = std::make_shared<CustomerManager>();
	//customer_->Init();
	//if (OrderManager::GetInstance().GetFirstOrder().drink_ == Order::DRINK::HOT)
	//{
	//	customer_->CreateCustomer(std::make_shared<HotCustomer>());
	//	customer_->Init();
	//}
	//else
	//{
	//	customer_->CreateCustomer(std::make_shared<IceCustomer>());
	//	customer_->Init();
	//}

	//カメラ
	mainCamera->SetFollow(&player_->GetTransform());
	mainCamera->ChangeMode(Camera::MODE::FOLLOW);

	timer_ = 20.0f;
}

void GameScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	Score& scr = Score::GetInstance();

#ifdef _DEBUG

	timer_ -= SceneManager::GetInstance().GetDeltaTime();

	//注文の制限時間がなくなったら新しく注文を生成し、
	//生成した注文に制限時間を設定
	//if (order.GetFirstOrder().orderTime_ < 0.01f)
	//{
	//	OrderManager::GetInstance().AddOrder()
	//	order_->SetTimer(OrderManager::GetInstance().GetOrderTime());

	//	//テストで注文が変わったら表示している客を一旦消して、
	//	//注文に合わせた客を再生成する
	//	customer_->ClearCustomers();
	//	if (OrderManager::GetInstance().GetOrder().drink_ == OrderManager::DRINK::HOT)
	//	{
	//		customer_->CreateCustomer(std::make_shared<HotCustomer>());
	//		customer_->Init();
	//	}
	//	else
	//	{
	//		customer_->CreateCustomer(std::make_shared<IceCustomer>());
	//		customer_->Init();
	//	}
	//}

	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		score_++;
	}

	//if (timer_ < 0.0f)
	//{
	//	scr.SaveScore(score_);
	//	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	//}

#endif // _DEBUG

	//シーン遷移
	if (ins.IsTrgDown(KEY_INPUT_N))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}

	if (order_->IsOrderTimeOut())
	{
		order_->ClearFirstOrder();
		
		customer_->ClearFirstCustomer();
		order_->AddOrder();
		customer_->AddCustomerByOrder();
	}

	skyDome_->Update();

	stage_->Update();

	player_->Update();

	order_->FirstOrderUpdate();

	customer_->Update();


}

void GameScene::Draw(void)
{
#ifdef _DEBUG

	DebugDraw();

#endif // _DEBUG

	//背景
	//skyDome_->Draw();
	stage_->Draw();
	
	player_->Draw();

	order_->Draw();

	customer_->Draw();
}

void GameScene::DebugDraw(void)
{
	//グリッド線描画
	VECTOR sPos;
	VECTOR ePos;
	float LEN = 1200.0f;
	float HLEN = LEN / 2.0f;
	float num = 0.0f;
	for (int z = -10; z < 10; z++)
	{
		num = static_cast<float>(z);
		sPos = { -HLEN, 0.0f, num * 50.0f };
		ePos = { HLEN, 0.0f, num * 50.0f };
		DrawLine3D(sPos, ePos, 0xff0000);
	}

	for (int x = -12; x < 12; x++)
	{
		num = static_cast<float>(x);
		sPos = { num * 50.0f, 0.0f, -HLEN };
		ePos = { num * 50.0f, 0.0f,  HLEN };
		DrawLine3D(sPos, ePos, 0x0000FF);
	}

	sPos = { 0.0f,  -HLEN, 0.0f };
	ePos = { 0.0f, HLEN, 0.0f };
	DrawLine3D(sPos, ePos, 0x00FF00);
	
	int line = 0;	//行
	int lineHeight = 30;	//行
	//左上から
	DebugDrawFormat::FormatString("tiem : %2.f", timer_, line++);
	SetFontSize(24);
	DebugDrawFormat::FormatString("注文数 : %d", order_->GetFirstOrder().num_, line++, lineHeight);
	DebugDrawFormat::FormatString("注文 : %d,%d", order_->GetFirstOrder().drink_, order_->GetFirstOrder().sweets_, line++, lineHeight);
	DebugDrawFormat::FormatString("注文制限時間 : %2.f", order_->GetFirstOrder().time_, line++, lineHeight);
	DebugDrawFormat::FormatString("%d個目", order_->GetCount(), line++, lineHeight);
	DebugDrawFormat::FormatString("%d人", customer_->GetCustomerNum(), line++, lineHeight);
	SetFontSize(16);

}
