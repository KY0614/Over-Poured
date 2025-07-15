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
#include "GameScene.h"

GameScene::GameScene(void)
{
	phase_ = PHASE::COUNT_DOWN;
	player_ = nullptr;
	stage_ = nullptr;
	customer_ = nullptr;
	timer_ = nullptr;
	score_ = 0;
	numbersImgs_ = nullptr;
	countImgs_ = nullptr;
	cntDownIdx_ = 0;
	cntDownTimer_ = 0.0f;
	scale_ = 0.0f;
	sclTime_ = 0.0f;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	auto& sound = SoundManager::GetInstance();
	//bgm�ǉ�&�Đ�
	sound.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAME,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_BGM).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GAME, 256 / 3);
	sound.Play(SoundManager::SOUND::GAME);

	sound.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GAME_FINISH,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_FINISH).handleId_);
	sound.AdjustVolume(SoundManager::SOUND::GAME_FINISH, 256 / 3);

	//�X�R�A�p�摜
	numbersImgs_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SCORE_NUMBER).handleIds_;

	//�J�E���g�_�E���p�摜
	countImgs_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::COUNTDOWN_NUMBER).handleIds_;

	//�^�C���A�b�v�p�摜
	timeUpImg_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::TIME_UP).handleId_;

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
	timer_ = std::make_unique<Timer>(MAX_MINUTE_TIME, MAX_SECOND_TIME);

	//�ŏ��̂��q�̒������󂯎��
	stage_->SetCurrentOrder(customer_->GetOrderData());

	//�J����
	mainCamera->SetFollow(&player_->GetTransform());
	//mainCamera->ChangeMode(Camera::MODE::FOLLOW);
	mainCamera->ChangeMode(Camera::MODE::TOP_FIXED);

}

void GameScene::Update(void)
{
	ScoreManager& scr = ScoreManager::GetInstance();

	switch (phase_)
	{
	case GameScene::PHASE::COUNT_DOWN:
		cntDownTimer_++;
		sclTime_ += SceneManager::GetInstance().GetDeltaTime();
		scale_ = Easing::QuintOut(
			static_cast<float>(sclTime_),
			static_cast<float>(1.0f), 0.0f, 2.0f);
		scale_ = std::clamp(abs(scale_),0.0f,2.0f);
		if (cntDownTimer_ >= COUNTDOWN_FRAME)
		{
			cntDownTimer_ = 0;
			cntDownIdx_++;
			sclTime_ = 0.0f;
			scale_ = 0.0f;
			if (cntDownIdx_ >= MAX_COUNT_DOWN)
			{
				phase_ = PHASE::GAME;
			}
		}
		break;
	
	case GameScene::PHASE::GAME:
		UpdateGame();
		break;
	
	case GameScene::PHASE::FINISH:
		SoundManager::GetInstance().Play(SoundManager::SOUND::GAME_FINISH);
		cntDownTimer_++;
		sclTime_ += SceneManager::GetInstance().GetDeltaTime();
		scale_ = Easing::QuintOut(
			static_cast<float>(sclTime_),
			static_cast<float>(1.0f), 0.0f, 2.0f);
		scale_ = std::clamp(abs(scale_), 0.0f, 2.0f);
		if (cntDownTimer_ >= COUNTDOWN_FRAME)
		{
			if (cntDownIdx_ >= MAX_COUNT_DOWN)
			{
				scr.SetCurrentScore(score_);
				scr.SaveScore(score_);
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
			}
			cntDownTimer_ = 0;
			cntDownIdx_++;
			sclTime_ = 0.0f;
			scale_ = 0.0f;
		}
		break;
	
	default:
		break;
	}
}

void GameScene::Draw(void)
{
	switch (phase_)
	{
	case GameScene::PHASE::COUNT_DOWN:
		//�X�e�[�W�`��
		stage_->Draw();
		//�v���C���[�`��
		player_->Draw();

		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2 - 50,
			scale_,
			0.0,
			countImgs_[cntDownIdx_],
			true
		);
		break;

	case GameScene::PHASE::GAME:
		DrawGame();
		break;

	case GameScene::PHASE::FINISH:
		DrawGame();
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2 - 50,
			scale_,
			0.0,
			timeUpImg_,
			true
		);
		break;

	default:
		break;
	}

}

void GameScene::UpdateGame(void)
{
	InputManager& ins = InputManager::GetInstance();
	ScoreManager& scr = ScoreManager::GetInstance();
	UIManager& ui = UIManager::GetInstance();

	if (customer_->GetIsMoving())
	{
		//�ŏ��̂��q�̒������󂯎��
		stage_->SetCurrentOrder(customer_->GetOrderData());
	}

	if (ins.IsInputTriggered("pause"))
	{
		//stop = !stop;
		score_ += 100;
	}

	if (stage_->IsServed())
	{
		//�X�R�A�̉��Z
		score_ += customer_->CheckServeAndOrder(stage_->GetServeItems());
		customer_->IsServe();	//�������o��
		stage_->ResetServeData();	//�T�[�u�����A�C�e�������Z�b�g
	}

	if (timer_->IsEnd())
	{
		phase_ = PHASE::FINISH;
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

void GameScene::DrawGame(void)
{
	//�X�e�[�W�`��
	stage_->Draw();
	//���q�ƒ����`��
	customer_->Draw();
	//�v���C���[�`��
	player_->Draw();
	//UI�`��
	UIManager::GetInstance().Draw();
	//�^�C�}�[�`��
	timer_->Draw();
	//�X�R�A�`��
	DrawScore(score_);
}

void GameScene::DrawScore(int score)
{
	std::string str = std::to_string(score);
	const int digitWidth = 70;
	for (int i = 0; i < str.size(); ++i)
	{
		char ch = str[i];
		if ('0' <= ch && ch <= '9')
		{
			int digit = ch - '0';
			DrawRotaGraph(
				50 + i * digitWidth, 50,
				0.8f,0.0f,
				numbersImgs_[digit], true);
		}
	}
}
