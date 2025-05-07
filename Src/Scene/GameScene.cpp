#include <DxLib.h>
#include <math.h>
#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include"../Manager/GameSystem/OrderManager.h"
#include"../Manager/GameSystem/OrderCustomerManager.h"
#include"../Object/Customer/CustomerBase.h"
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
	customer_ = nullptr;
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

	customer_ = std::make_unique<OrderCustomerManager>();
	customer_->Init();

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

	VECTOR spPos = { 221.0f, 0.0f, 139.0f };
	int r = 30;
	if (!customer_->GetIsMoving()&&
		AsoUtility::IsHitSpheres(spPos, r, player_->GetCapsule().GetPosDown(), 20))
	{
		if (ins.IsTrgDown(KEY_INPUT_SPACE))
		{
	 		score_ += customer_->CheckServeAndOrder(player_->GetPlayerItem());
			customer_->IsServe();
		}
	}

	//if (ins.IsTrgDown(KEY_INPUT_SPACE))
	//{
	//	score_++;
	//}

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

	skyDome_->Update();

	stage_->Update();

	player_->Update();

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
	DebugDrawFormat::FormatString(L"tiem : %2.f", timer_, line);
	DebugDrawFormat::FormatString(L"", timer_, line);
	DebugDrawFormat::FormatStringRight(L"score : %d", score_, line);

}
