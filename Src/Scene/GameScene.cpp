#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Planet.h"
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

	// �v���C���[
	player_ = std::make_shared<Player>();
	player_->Init();

	// �X�e�[�W
	stage_ = std::make_unique<Stage>(*player_);
	stage_->Init();

	// �X�e�[�W�̏����ݒ�
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// �X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	mainCamera->SetFollow(&player_->GetTransform());
	mainCamera->ChangeMode(Camera::MODE::FIXED_POINT);

	player_->SetTime(5.0f);
	timer_ = 10.0f;
}

void GameScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	Score& scr = Score::GetInstance();

	timer_ -= SceneManager::GetInstance().GetDeltaTime();

	if (timer_ < 0.0f)
	{
		scr.SaveScore(score_);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		score_++;
	}

	// �V�[���J��
	if (ins.IsTrgDown(KEY_INPUT_N))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}

	skyDome_->Update();

	stage_->Update();

	player_->Update();
}

void GameScene::Draw(void)
{
	DebugDraw();
	// �w�i
	//skyDome_->Draw();
	stage_->Draw();
	
	player_->Draw();

}

void GameScene::DebugDraw(void)
{
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

	DrawFormatString(0, 0, 0xff0000, "tiem : %2.f", timer_);
	DrawFormatString(0, 20, 0xff0000, "score : %d", score_);
}
