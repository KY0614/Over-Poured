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
#include"../Object/Order/OrderManager.h"
#include "../Object/Player.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene(void)
{
	player_ = nullptr;
	stage_ = nullptr;
	customer_ = nullptr;
}

TutorialScene::~TutorialScene(void)
{

}

void TutorialScene::Init(void)
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
	for (int i = 0; i < tableNum; ++i)
	{
		player_->AddCollider(stage_->GetTableTran(i).collider);
	}
	player_->AddCollider(stage_->GetShowCase().collider);
	player_->AddCollider(stage_->GetDustBox().collider);
	player_->AddCollider(stage_->GetFloorTran().collider);

	//�q�ƒ���
	customer_ = std::make_unique<OrderCustomerManager>();
	customer_->Init();

	//�ŏ��̂��q�̒������󂯎��
	stage_->SetCurrentOrder(customer_->GetOrderData());

	//�J����
	mainCamera->SetFollow(&player_->GetTransform());
	mainCamera->ChangeMode(Camera::MODE::TOP_FIXED);

}

void TutorialScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (customer_->GetIsMoving())
	{
		//�ŏ��̂��q�̒������󂯎��
		stage_->SetCurrentOrder(customer_->GetOrderData());
	}

	if (ins.IsInputTriggered("pause"))
	{
	
	}

#ifdef _DEBUG

	if (stage_->IsServed())
	{
		customer_->IsServe();	//�������o��
		stage_->ResetServeData();	//�T�[�u�����A�C�e�������Z�b�g
	}

#endif // _DEBUG

	//if (timer_->IsEnd())
	//{
	//	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	//}

	//�V�[���J��
	if (ins.IsInputTriggered("NextScene"))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}

	stage_->Update();

	player_->Update();

	customer_->Update();
}

void TutorialScene::Draw(void)
{
	//�X�e�[�W�`��
	stage_->Draw();
	//���q�ƒ����`��
	customer_->Draw();
	//�v���C���[�`��
	player_->Draw();
}
