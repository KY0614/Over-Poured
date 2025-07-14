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
	//�v���C���[
	player_ = std::make_unique<Player>();
	player_->Init();

	//�X�e�[�W
	stage_ = std::make_unique<StageManager>(*player_);
	stage_->Init();

	//�X�e�[�W�̃R���C�_�[��ǉ�
	player_->AddCollider(stage_->GetCounterTran().collider);
	int tableNum = stage_->GetTableNum();
	for(int i = 0; i < tableNum; ++i)
	{
		player_->AddCollider(stage_->GetTableTran(i).collider);
	}
	player_->AddCollider(stage_->GetShowCase().collider);
	player_->AddCollider(stage_->GetDustBox().collider);
	player_->AddCollider(stage_->GetFloorTran().collider);

	//�q�ƒ���
	customer_ = std::make_unique<OrderCustomerManager>();
	customer_->Init();

	//�^�C�}�[(2:00�j�Q��
	timer_ = std::make_unique<Timer>(MAX_MINUTE_TIME,0);

	//�ŏ��̂��q�̒������󂯎��
	stage_->SetCurrentOrder(customer_->GetOrderData());

	//�J����
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
		//�ŏ��̂��q�̒������󂯎��
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
		//�X�R�A�̉��Z
		score_ += customer_->CheckServeAndOrder(stage_->GetServeItems());
		customer_->IsServe();	//�������o��
		stage_->ResetServeData();	//�T�[�u�����A�C�e�������Z�b�g
	}

#endif // _DEBUG

	if (timer_->IsEnd())
	{
		scr.SetCurrentScore(score_);
		scr.SaveScore(score_);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}

	//�V�[���J��
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
	//�X�e�[�W�`��
	stage_->Draw();
	//���q�ƒ����`��
	customer_->Draw();
	//�v���C���[�`��
	player_->Draw();
	UIManager::GetInstance().Draw();
	//�^�C�}�[�`��
	timer_->Draw();
}