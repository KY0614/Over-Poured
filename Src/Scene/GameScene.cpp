#include <DxLib.h>
#include <math.h>
#include "../Common/DebugDrawFormat.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include"../Manager/GameSystem/OrderCustomerManager.h"
#include"../Object/Customer/CustomerBase.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/Stage/StageObjectLibrary.h"
#include "../Object/Stage/StageManager.h"
#include"../Object/Order/OrderManager.h"
#include "../Object/Player.h"
#include "../Object/Score.h"
#include "../Object/SkyDome.h"
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
	//�v���C���[
	player_ = std::make_shared<Player>();
	player_->Init();

	Vector2 size = { 1000,1000 };
	//�X�e�[�W
	stage_ = std::make_unique<StageManager>(size,*player_);
	stage_->Init();

	//�X�e�[�W�̃R���C�_�[��ǉ�
	player_->AddCollider(stage_->GetTransform().collider);

	//�X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	//�q�ƒ���
	customer_ = std::make_unique<OrderCustomerManager>();
	customer_->Init();

	//�J����
	mainCamera->SetFollow(&player_->GetTransform());
	mainCamera->ChangeMode(Camera::MODE::FOLLOW);

	//�^�C�}�[�̐ݒ�
	timer_ = MAX_TIME;
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
			stage_->ResetHotCup();
		}
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
		scr.SetCurrentScore(score_);
		scr.SaveScore(score_);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
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
	int lineHeight = 50;	//�s
	SetFontSize(24);
	//���ォ��
	DebugDrawFormat::FormatString(L"                                     time : %2.f", timer_, line);
	DebugDrawFormat::FormatStringRight(L"score : %d", score_, line, lineHeight);
	SetFontSize(16);
}
