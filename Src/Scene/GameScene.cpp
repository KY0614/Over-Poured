#include <DxLib.h>
#include <math.h>
#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include"../Manager/GameSystem/OrderManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Score.h"
#include "../Object/Customer/CustomerManager.h"
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
	//�v���C���[
	player_ = std::make_shared<Player>();
	player_->Init();

	//�X�e�[�W
	stage_ = std::make_unique<Stage>(*player_);
	stage_->Init();

	//�X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	//����
	OrderManager::GetInstance().InitOrder();

	customer_ = std::make_shared<CustomerManager>();
	customer_->Init();
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

	//�J����
	mainCamera->SetFollow(&player_->GetTransform());
	mainCamera->ChangeMode(Camera::MODE::FOLLOW);

	timer_ = 20.0f;
}

void GameScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	Score& scr = Score::GetInstance();
	OrderManager& order = OrderManager::GetInstance();

	OrderManager::GetInstance().OrderUpdate();

#ifdef _DEBUG

	timer_ -= SceneManager::GetInstance().GetDeltaTime();

	//�����̐������Ԃ��Ȃ��Ȃ�����V���������𐶐����A
	//�������������ɐ������Ԃ�ݒ�
	//if (order.GetFirstOrder().orderTime_ < 0.01f)
	//{
	//	OrderManager::GetInstance().AddOrder()
	//	order_->SetTimer(OrderManager::GetInstance().GetOrderTime());

	//	//�e�X�g�Œ������ς������\�����Ă���q����U�����āA
	//	//�����ɍ��킹���q���Đ�������
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

	//�V�[���J��
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

	OrderManager::GetInstance().Draw();

	//�w�i
	//skyDome_->Draw();
	stage_->Draw();
	
	player_->Draw();

	customer_->Draw();
}

void GameScene::DebugDraw(void)
{
	//�O���b�h���`��
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
	
	int line = 0;	//�s
	int lineHeight = 30;	//�s
	auto order = OrderManager::GetInstance().GetFirstOrder();
	//���ォ��
	DebugDrawFormat::FormatString("tiem : %2.f", timer_, line++);
	SetFontSize(24);
	DebugDrawFormat::FormatString("������ : %d", order.num_, line++, lineHeight);
	DebugDrawFormat::FormatString("���� : %d,%d", order.drink_, order.sweets_, line++, lineHeight);
	DebugDrawFormat::FormatString("������������ : %2.f", order.orderTime_, line++, lineHeight);
	DebugDrawFormat::FormatString("%d��", OrderManager::GetInstance().GetCount(), line++, lineHeight);
	DebugDrawFormat::FormatString("%d�l", customer_->GetCustomerNum(), line++, lineHeight);
	SetFontSize(16);

	DebugDrawFormat::FormatString("pos : %2.f,%2.f,%2.f", customer_->GetPos() , line++,lineHeight);

	//DrawFormatString(0, 0, 0xff0000, "tiem : %2.f", timer_);
	//SetFontSize(24);
	//DrawFormatString(0, 40, 0xff0000, "������ : %d", OrderManager::GetInstance().GetOrderNum());
	//DrawFormatString(0, 70, 0xff0000, "���� : %d,%d", order.drink_,order.sweets_);
	//DrawFormatString(0, 100, 0xff0000, "������������ : %2.f", order_->GetTimer());
	//SetFontSize(16);

}
